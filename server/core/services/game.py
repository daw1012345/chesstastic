from collections import OrderedDict
from typing import Optional
from textwrap import wrap

from django.db.models import Q
from django.db.models.query import QuerySet

from core.models import Game, Player, Turn
import core.services.gamelogic
import core.services.player
import core.services.bot


def get_player_games(player: Player) -> QuerySet:
    """
    Return all games that belong to a given player.
    """
    return Game.objects.filter(Q(white=player) | Q(black=player))


def get_player_active_game(player: Player) -> Optional[Game]:
    """
    Get the currently active game of a player.
    """
    return get_player_games(player).exclude(game_over=True).first()


def serialize_board(board: core.services.gamelogic.Board) -> dict:
    """
    Return a dictionary of the board that is suitable for JSON serialization.
    """
    result = {
        'state': [],
        'moves': [],
    }
    for position, piece in board.state.items():
        coords = core.services.gamelogic.position_to_coordinates(position)
        result['state'].append({
            'piece': {
                'type': piece.type.name.lower(),
                'color': piece.color.name.lower(),
            } if piece else None,
            'position': {
                'index': position,
                'coordinates': {'row': coords.row, 'col': coords.col},
                'algebraic': core.services.gamelogic.position_to_algebraic(position),
            },
        })
    for move in board.moves:
        result['moves'].append({
            'color': move.color.name.lower(),
            'source': move.source,
            'target': move.target,
            'move_type': move.move_type.value,
        })
    return result


def serialize_replay(game: Game) -> dict:
    game_board = get_board_from_game(game)
    engine = core.services.bot.get_engine()
    steps = []
    best_moves = []
    board = core.services.gamelogic.Board(moves=[], state={})
    game_board.moves = [None] + game_board.moves
    for move in game_board.moves:
        if move:
            board.moves.append(move)
        board = core.services.gamelogic.recalculate_board_state(board)
        core.services.bot.set_engine_board(engine, board)
        steps.append(board.state)
        bm_source_alg, bm_target_alg = wrap(str(core.services.bot.get_best_move(
            engine,
            max_time=500,
        )), 2)
        best_moves.append({
            'algebraic': f'{bm_source_alg}{bm_target_alg}',
            'source_index': core.services.gamelogic.algebraic_to_position(bm_source_alg),
            'target_index': core.services.gamelogic.algebraic_to_position(bm_target_alg),
        })
    for i, step in enumerate(steps):
        steps[i] = {
            'state': [
                {
                    'type': piece.type.name.lower(),
                    'color': piece.color.name.lower(),
                } if piece else None for piece in step.values()
            ],
            'best_move': best_moves[i],
        }
    return {
        'steps': steps,
    }


def get_board_from_game(game: Game) -> core.services.gamelogic.Board:
    """
    Take a game, and return corresponding board.
    """
    board = core.services.gamelogic.Board(moves=[], state=[])
    for turn in game.turn_set.all().order_by('pk'):
        by_white = turn.player == game.white
        color = core.services.gamelogic.Color.WHITE if \
                by_white else core.services.gamelogic.Color.BLACK
        source = turn.source_field
        target = turn.destination_field
        board.moves.append(core.services.gamelogic.Move(
            color=color,
            source=core.services.gamelogic.Position(source) if source else None,
            target=core.services.gamelogic.Position(target) if target else None,
            move_type=core.services.gamelogic.MoveType(turn.move_type),
        ))
    return core.services.gamelogic.recalculate_board_state(board)


def parse_move(
    raw_move: dict,
    color: core.services.gamelogic.Color,
    ) -> core.services.gamelogic.Move:
    """
    Takes in a dictionary that is supposed to represent a move and tries to convert it.
    May throw a ValueError if things don't work out. Is prepared to deal with unsafe user
    input.
    """
    try:
        move_type = core.services.gamelogic.MoveType(raw_move['move_type'])
        source = core.services.gamelogic.Position(
            raw_move['source']
        ) if 'source' in raw_move else None
        target = core.services.gamelogic.Position(
            raw_move['target']
        ) if 'target' in raw_move else None
    except (KeyError, ValueError):
        raise ValueError()
    return core.services.gamelogic.Move(
        color=color,
        source=source,
        target=target,
        move_type=move_type,
    )


def is_gamemover(game: Game) -> bool:
    board = get_board_from_game(game)
    if len(board.moves) > 0:
        if board.moves[-1].move_type == core.services.gamelogic.MoveType.RESIGNATION:
            return True
    return (
        bool(core.services.gamelogic.is_checkmate(board)) or
        core.services.gamelogic.is_stalemate(board) or
        core.services.gamelogic.is_exhausted(board)
    )


def update_game_db_status(game: Game) -> None:
    """
    Updated calculated game fields in the database.
    """
    board = get_board_from_game(game)
    game_over = is_gamemover(game)
    game.game_over = game_over
    if game_over:
        board = get_board_from_game(game)
        if board.moves[-1].move_type == core.services.gamelogic.MoveType.RESIGNATION:
            if board.moves[-1].color == core.services.gamelogic.Color.WHITE:
                game.winner = game.black
            else:
                game.winner = game.white
        else:
            checkmate = core.services.gamelogic.is_checkmate(board)
            if checkmate == core.services.gamelogic.Color.WHITE:
                game.winner = game.white
            elif checkmate == core.services.gamelogic.Color.BLACK:
                game.winner = game.black
            else:
                game.winner = None
    else:
        game.winner = None
    game.save()


def save_turn(game: Game, player: Player, move: core.services.gamelogic.Move) -> None:
    """
    Save a Move as a Turn in the database. Then update the game status in the database.
    """
    Turn(
        game=game,
        move_type=move.move_type.value,
        source_field=move.source,
        destination_field=move.target,
        player=player, 
    ).save()
    update_game_db_status(game)
    game = Game.objects.get(pk=game.pk)  # Refresh the game from DB by re-querying
    if game.game_over:
        # The game just ended
        core.services.player.process_elo(game.white, game.black, game.winner)
        return
    opponent = game.white if game.black == player else game.black
    if opponent.bot and not player.bot:  # Detect bot, disallow fighting bots
        core.services.bot.save_next_move(opponent, game)


def stringboard_to_board(stringboard: str) -> core.services.gamelogic.Board:
    """
    Takes in a string representation of a chess board, returns a Board with that state and
    no moves.

    (blacks)
    RKBQXBKR
    PPPPPPPP
    ........
    ........
    ........
    ........
    pppppppp
    rkbqxbkr
    (whites)
    """
    board = core.services.gamelogic.Board(moves=[], state={})
    stringboard = ''.join(reversed([x.strip() for x in stringboard.strip().split('\n')]))
    for char, idx in zip(stringboard, range(1, 65)):
        position = core.services.gamelogic.Position(idx)
        color = core.services.gamelogic.Color.WHITE if char.islower() else \
            core.services.gamelogic.Color.BLACK
        piece_type = core.services.gamelogic.PieceType.from_letter(char)
        if piece_type:
            board.state[position] = core.services.gamelogic.Piece(
                color=color,
                type=piece_type,
            )
        else:
            board.state[position] = None
    return board


def board_to_stringboard(board: core.services.gamelogic.Board) -> str:
    """
    Opposite of stringboard_to_board().
    """
    state = OrderedDict(sorted(board.state.items()))
    result = []
    for piece in state.values():
        letter = core.services.gamelogic.PieceType.to_letter(piece.type if piece else None)
        if piece and piece.color == core.services.gamelogic.Color.BLACK:
            letter = letter.upper()
        result.append(letter)
    result = '\n'.join(reversed(wrap(''.join(result), 8)))
    return result
