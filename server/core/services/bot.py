import chess
from stockfish import Stockfish
from textwrap import wrap

from django.conf import settings

from core.models import Game, Player
import core.services.game
import core.services.gamelogic


def get_engine() -> Stockfish:
    engine = Stockfish(settings.STOCKFISH_EXECUTABLE)
    engine.set_depth(3)
    engine.set_skill_level(1)
    return engine


def set_engine_board(engine: Stockfish, board: core.services.gamelogic.Board) -> Stockfish:
    """
    Sets the engines' internal board state according to FEN[1] notation.

    [1] https://nl.wikipedia.org/wiki/Forsyth-Edwards_Notation
    """
    stringboard = core.services.game.board_to_stringboard(board)
    positions = '/'.join(stringboard.split('\n')) \
                   .swapcase().replace('.', '1') \
                   .replace('k', 'n').replace('K', 'N') \
                   .replace('x', 'k').replace('X', 'K')
    active_player = 'w' if core.services.gamelogic.get_current_color(board) == \
        core.services.gamelogic.Color.WHITE else 'b'
    moves_count = len(board.moves)
    engine.set_fen_position(
        f'{positions} {active_player} - - 0 {moves_count}'
    )
    return engine


def get_best_move(
    engine: Stockfish,
    color: core.services.gamelogic.Color = None,
    max_time: int = 1000,
) -> core.services.gamelogic.Move:
    """
    Returns the best possible move according to the chess engine.
    """
    move = engine.get_best_move_time(max_time)
    source_algebraic, target_algebraic = wrap(move, 2)
    source = core.services.gamelogic.algebraic_to_position(source_algebraic)
    target = core.services.gamelogic.algebraic_to_position(target_algebraic)
    return core.services.gamelogic.Move(
        color=color if color else core.services.gamelogic.Color.WHITE,
        move_type=core.services.gamelogic.MoveType.REGULAR,
        source=source,
        target=target,
    )


def save_next_move(bot_player: Player, game: Game) -> None:
    engine = get_engine()
    set_engine_board(
        engine,
        core.services.game.get_board_from_game(game),
    )
    is_white = bot_player == game.white
    color = core.services.gamelogic.Color.WHITE if is_white else \
        core.services.gamelogic.Color.BLACK
    core.services.game.save_turn(
        game,
        bot_player,
        get_best_move(engine, color)
    )


def lib_validate_move(
    board: core.services.gamelogic.Board,
    move: core.services.gamelogic.Move,
) -> bool:
    board.moves.append(move)
    chess_board = chess.Board()
    try:
        for move in board.moves:
            chess_board.push_uci(str(move).lower())
    except ValueError:
        return False
    return True


def lib_validate_checkmate(board: core.services.gamelogic.Board) -> bool:
    chess_board = chess.Board()
    for move in board.moves:
        chess_board.push_uci(str(move).lower())
    return chess_board.is_checkmate()


def lib_validate_stalemate(board: core.services.gamelogic.Board) -> bool:
    chess_board = chess.Board()
    for move in board.moves:
        chess_board.push_uci(str(move).lower())
    return chess_board.is_stalemate()


def lib_validate_exhausted(board: core.services.gamelogic.Board) -> bool:
    chess_board = chess.Board()
    for move in board.moves:
        chess_board.push_uci(str(move).lower())
    return chess_board.is_game_over() and not lib_validate_checkmate(board) and not \
        lib_validate_stalemate(board)
