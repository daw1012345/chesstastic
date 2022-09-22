from enum import Enum
from typing import Tuple, List, Optional, Dict
from collections import OrderedDict, defaultdict
from dataclasses import dataclass

from django.conf import settings

from core.models import Turn
import core.services.game


class PieceType(Enum):

    PAWN = 'Pawn'
    ROOK = 'Rook'
    BISHOP = 'Bishop'
    KNIGHT = 'Knight'
    KING = 'King'
    QUEEN = 'Queen'

    @staticmethod
    def from_letter(letter: str) -> Optional['PieceType']:
        letter = letter.lower()
        PIECETYPE_MAP = {
            'p': PieceType.PAWN,
            'r': PieceType.ROOK,
            'k': PieceType.KNIGHT,
            'b': PieceType.BISHOP,
            'q': PieceType.QUEEN,
            'x': PieceType.KING,
        }
        return PIECETYPE_MAP.get(letter)

    @staticmethod
    def to_letter(piece_type: Optional['PieceType']) -> str:
        if not piece_type:
            return '.'
        if piece_type == PieceType.KING:
            return 'x'
        return piece_type.name.lower()[0]


class MoveType(Enum):

    REGULAR = Turn.REGULAR
    CASTLE_LONG = Turn.CASTLE_LONG
    CASTLE_SHORT = Turn.CASTLE_SHORT
    PROMOTION_BISHOP = Turn.PROMOTION_BISHOP
    PROMOTION_QUEEN = Turn.PROMOTION_QUEEN
    PROMOTION_KNIGHT = Turn.PROMOTION_KNIGHT
    PROMOTION_ROOK = Turn.PROMOTION_ROOK
    RESIGNATION = Turn.RESIGNATION


class Color(Enum):

    BLACK = 'Black'
    WHITE = 'White'

    @staticmethod
    def opposite(color: 'Color'):
        if color == Color.WHITE:
            return Color.BLACK
        return Color.WHITE


class Position(int):

    pass


@dataclass
class Piece:

    color: Color
    type: PieceType


@dataclass
class Move:

    color: Color
    source: Optional[Position]
    target: Optional[Position]
    move_type: MoveType

    def __str__(self):
        """
        Returns UCI notation of the move.
        """
        if self.move_type in [MoveType.CASTLE_LONG, MoveType.CASTLE_SHORT]:
            if self.color == Color.WHITE:
                if self.move_type == MoveType.CASTLE_LONG:
                    return 'e1c1'
                elif self.move_type == MoveType.CASTLE_SHORT:
                    return 'e1g1'
            else:
                if self.move_type == MoveType.CASTLE_LONG:
                    return 'e8c8'
                elif self.move_type == MoveType.CASTLE_SHORT:
                    return 'e8g8'
        promotion = ''
        if self.move_type in [
            MoveType.PROMOTION_BISHOP,
            MoveType.PROMOTION_KNIGHT,
            MoveType.PROMOTION_QUEEN,
            MoveType.PROMOTION_ROOK,
        ]:
            promotion = self.move_type.name.lower().split('promotion_')[1][0]
        if self.source and self.target:
            source = position_to_algebraic(self.source)
            target = position_to_algebraic(self.target)
            return f'{source}{target}{promotion}'
        return super().__str__()


@dataclass
class Board:

    moves: List[Move]
    state: Dict[Position, Optional[Piece]]


@dataclass
class Coordinate:

    row: int
    col: int


class Algebraic(str):

    pass


def algebraic_to_position(an: Algebraic) -> Position:
    """
    Resolves algebraic notation ('A3') to a single position integer (1-64).
    """
    position = int(an[1]) * 8 + ((ord(an[0].lower()) - 96) - 8)
    return Position(position)


def position_to_algebraic(idx: Position) -> Algebraic:
    """
    Resolves single position integer (1-64) to algebraic notation ('A3').
    """
    algebraic = (chr(idx % 8 + 96) if idx % 8 != 0 else 'h').upper() + str(int((idx - 1) / 8) + 1)
    return Algebraic(algebraic)


def position_to_coordinates(idx: Position) -> Coordinate:
    """
    Returns coordinates of the given position (1-8).
    """
    col = idx % 8 if idx % 8 != 0 else 8
    row = int((idx - 1) / 8) + 1
    return Coordinate(row=row, col=col)


def coordinates_to_position(coordinate: Coordinate) -> Position:
    """
    Returns single position integer (1-64) for a set of coordinates.
    """
    return coordinate.row * 8 + (coordinate.col - 8)


def get_bare_moves(piece: Optional[Piece], position: Position) -> List[Position]:
    """
    For any piece and position, assume an empty board and return all legal places where that
    piece can go.
    """
    if not piece:
        return []

    possible_moves = []
    coords = position_to_coordinates(position)

    if piece.type == PieceType.PAWN:
        has_moved = (
            piece.color == Color.BLACK and coords.row == 7 or
            piece.color == Color.WHITE and coords.row == 2
        )
        if piece.color == Color.WHITE:
            possible_moves.append(coordinates_to_position(Coordinate(col=coords.col, row=coords.row + 1)))
            if has_moved:
                possible_moves.append(coordinates_to_position(Coordinate(col=coords.col, row=coords.row + 2)))
        else:
            possible_moves.append(coordinates_to_position(Coordinate(col=coords.col, row=coords.row - 1)))
            if has_moved:
                possible_moves.append(coordinates_to_position(Coordinate(col=coords.col, row=coords.row - 2)))
    if piece.type == PieceType.KING:
        for i in range(-1, 2):
            for j in range(-1,2):
                if 0 < (coords.col + i) < 9 and 0 < (coords.row + j) < 9:
                    new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row + j))
                    if new_position != position:
                        possible_moves.append(new_position)
    if piece.type == PieceType.ROOK:
        for i in range(-7,8):
            if 0 < (coords.row + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
            if 0 < (coords.col + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row))
                if new_position != position:
                    possible_moves.append(new_position)
    if piece.type == PieceType.BISHOP:
        for i in range(-7,8):
            if 0 < (coords.row + i) < 9 and 0 < (coords.col + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
            if 0 < (coords.row + i) < 9 and 0 < (coords.col - i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col - i, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
    if piece.type == PieceType.KNIGHT:
        for i in range(-2,3): # because you can move col + 1, +2, -1, -2
            if i != 0 and 1%i == 0: # modulo is 0 for i = -1 & 1, so row will change 2.
                if 0 < (coords.col + i) < 9 and 0 < (coords.row + 2*i) < 9: # when 
                    new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row + 2))
                    new_position2 = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row - 2))
                    if new_position > 0:
                        possible_moves.append(new_position)
                    if new_position2 > 0 and coords.col < 9:
                        possible_moves.append(new_position2)
                if 0 < (coords.col - i) < 9 and 0 < (coords.row - 2*i) < 9:
                    new_position3 = coordinates_to_position(Coordinate(col=coords.col - i, row=coords.row + 2))
                    new_position4 = coordinates_to_position(Coordinate(col=coords.col - i, row=coords.row - 2))
                    if new_position3 > 0 and (coords.col - i) < 9:
                        possible_moves.append(new_position3)
                    if new_position4 > 0 and (coords.col -i) < 9:
                        possible_moves.append(new_position4) 
            elif i != 0:
                if 0 < (coords.col + i) < 9 and 0 < (coords.row + i) < 9: 
                    new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row + 1))
                    new_position2 = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row - 1))
                    if new_position > 0:
                        possible_moves.append(new_position)
                    if new_position2 > 0:
                        possible_moves.append(new_position2)
        possible_moves = sorted(set(possible_moves))
    elif piece.type == PieceType.QUEEN:
        for i in range(-7,8):
            if 0 < (coords.row + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
            if 0 < (coords.col + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row))
                if new_position != position:
                    possible_moves.append(new_position)
            if 0 < (coords.row + i) < 9 and 0 < (coords.col + i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col + i, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
            if 0 < (coords.row + i) < 9 and 0 < (coords.col - i) < 9:
                new_position = coordinates_to_position(Coordinate(col=coords.col - i, row=coords.row + i))
                if new_position != position:
                    possible_moves.append(new_position)
    return possible_moves


def is_threatened(position: Position, board: Board, color: Optional[Color] = None):
    """
    Returns if the piece on specified position is threatened.

    By default, computes for color of piece or any color if there is no piece
    on specified position.
    """
    if board.state[position] and not color:
        color = board.state[position].color
    if not color:
        return False
    for current_position, piece in board.state.items():
        if position in get_bare_moves(piece, current_position):
            return True
    return False


def can_castle(board: Board, color: Color) -> Tuple[bool, bool]:
    """
    Return if specified color can castle (long, short).
    """

    if is_check(board) == color:
        return False, False

    left_rook_position = Position (1) if color == color.WHITE else Position(57)
    right_rook_position = Position (8) if color == color.WHITE else Position(64)
    king_position = Position (5) if color == color.WHITE else Position(61)

    if list(filter(lambda x: x.source == king_position, board.moves)):
        return False, False
 
    if list(filter(lambda x: x.color == color and x.move_type in [
        MoveType.CASTLE_LONG,
        MoveType.CASTLE_SHORT,
    ], board.moves)):
        return False, False

    left_castle = (
        not list(filter(lambda x: x.source == left_rook_position, board.moves)) and
        not board.state[left_rook_position + 1] and
        not board.state[left_rook_position + 2] and
        not board.state[left_rook_position + 2]
    )
    right_castle = (
        not list(filter(lambda x: x.source == left_rook_position, board.moves)) and
        not board.state[right_rook_position - 1] and
        not board.state[right_rook_position - 2]
    )

    left_threaten_positions = [
        Position(2),
        Position(3),
        Position(4),
    ] if color == color.WHITE else [
        Position(58),
        Position(59),
        Position(60),
    ]
    right_threaten_positions = [
        Position(6),
        Position(7),
    ] if color == color.WHITE else [
        Position(62),
        Position(63)
    ]

    for position in left_threaten_positions:
        if is_threatened(position, board, Color.opposite(color)):
            left_castle = False
    for position in right_threaten_positions:
        if is_threatened(position, board, Color.opposite(color)):
            right_castle = False

    return left_castle, right_castle
 
 
def can_enpassant(board: Board, color: Color) -> List[Position]:
    """
    For specified color, return all positions that hold a pawn that can hit en passant.
    """
    last_move = board.moves[-1]
    pawns_list = []
    if (abs(last_move.source - last_move.target) == 16 
        and last_move.color == Color.opposite(color)
        and board.state[last_move.target].type == PieceType.PAWN):
        if (board.state[last_move.target + 1].type == PieceType.PAWN 
            and board.state[last_move.target + 1].color == color
            and last_move.target % 8 != 0):
            pawns_list.append(board.state[last_move.target + 1]) 
        if (board.state[last_move.target - 1].type == PieceType.PAWN 
            and board.state[last_move.target - 1].color == color
            and last_move.target % 8 != 1):
            pawns_list.append(board.state[last_move.target - 1])     
    return pawns_list


def is_check(board: Board) -> Optional[Color]: 
    """
    Return color that is in check or None.
    Someone is in check when the king is being attacked.
    """
    for position, piece in board.state.items():
        if piece and piece.type == PieceType.KING:
            if is_threatened(position, board):
                return piece.color
    

def is_checkmate(board: Board) -> Optional[Color]:
    """
    Return color that is mated or None.
    """
    if settings.LIBRARY_VALIDATION:
        if core.services.bot.lib_validate_checkmate(board):
            return board.moves[-1].color
    color_in_check = is_check(board)
    if not color_in_check or get_current_color(board) != color_in_check:
        return
    for position in board.state.keys():
        if get_moves(position, board):
            return False
    return True


def is_stalemate(board: Board) -> bool:
    """
    Return if the board is in a stalemate.
    
    A stalemate occurs whenever the player to move is both:
    1. not in check
    2. unable to make any legal move
    """
    if settings.LIBRARY_VALIDATION:
        return core.services.bot.lib_validate_stalemate(board)
    if is_check(board):
        return False
    color_to_move = get_current_color(board)
    for position, piece in board.state.items():
        if piece and piece.color == color_to_move and get_moves(position, board):
            return False
    return True


def is_exhausted(board: Board) -> bool:
    """
    Return True if the board is in a state of:
     * Threefold repetition; or
     * Insufficient material
    """

    if settings.LIBRARY_VALIDATION:
        return core.services.bot.lib_validate_exhausted(board)

    # Threefold repetition
    temp_board = Board(moves=[], state={})
    stringboards = []
    for move in board.moves:
        temp_board.moves.append(move)
        recalculate_board_state(temp_board)
        stringboards.append(core.services.game.board_to_stringboard(temp_board))
    occurences = defaultdict(int)
    for strinboard in stringboards:
        occurences[strinboard] += 1
    if list(occurences.values()) and max(list(occurences.values())) > 3:
        return True
    
    # Insufficient material
    black_pieces = [
        piece for piece in board.state.values() if piece and piece.color == Color.BLACK
    ]
    if len(black_pieces) < 3:
        return (
            Piece(color=Color.BLACK, type=PieceType.BISHOP) not in black_pieces and
            Piece(color=Color.BLACK, type=PieceType.KNGIHT) not in black_pieces
        )
    white_pieces = [
        piece for piece in board.state.values() if piece and piece.color == Color.WHITE
    ]
    if len(white_pieces) < 3:
        return (
            Piece(color=Color.WHITE, type=PieceType.BISHOP) not in white_pieces and
            Piece(color=Color.WHITE, type=PieceType.KNGIHT) not in white_pieces
        )

    return False


def get_current_color(board: Board) -> Color:
    """
    Return whose turn it is.
    """
    if len(board.moves) == 0 or board.moves[-1].color == Color.BLACK:
        return Color.WHITE
    return Color.BLACK


def get_moves(position: Position, board: Board) -> List[Position]:
    """
    For the piece at given position, calculate all possible legal positions.

    This method will make sure moving the piece does not put own king in check.
    """
    piece = board.state[position]
    if not piece:
        return []
    return get_bare_moves(piece, position)


def validate_move(move: Move, board: Board) -> Optional[str]:
    """
    Verify a move. Return None if it's a valid move, or an error message.
    """
    if move.color != get_current_color(board):
        return 'It is not your turn!'
    if is_checkmate(board) or is_stalemate(board) or is_exhausted(board):
        return 'This game is already over!'
    if settings.LIBRARY_VALIDATION:
        return 'Invalid move' if not core.services.bot.lib_validate_move(board, move) else None
    if move.move_type == MoveType.RESIGNATION:
        return
    if move.move_type in [
        MoveType.REGULAR,
        MoveType.CASTLE_LONG,
        MoveType.CASTLE_SHORT,
        MoveType.PROMOTION_BISHOP,
        MoveType.PROMOTION_KNIGHT,
        MoveType.PROMOTION_QUEEN,
        MoveType.PROMOTION_ROOK,
    ]:
        if not move.source and move.target:
            return 'Malformed move'
        if move.target not in get_moves(move.source, board):
            return 'The pieces\' destination is invalid'
    if move.move_type == MoveType.CASTLE_LONG:
        return can_castle(board, move.color)[0]
    if move.move_type == MoveType.CASTLE_SHORT:
        return can_castle(board, move.color)[1]


def recalculate_board_state(board: Board) -> Board:
    """
    For the given board, recalculate its state. Leave the move list untouched
    and return the board with new state.
    """

    state = [

        (Position(1), Piece(color=Color.WHITE, type=PieceType.ROOK)),
        (Position(2), Piece(color=Color.WHITE, type=PieceType.KNIGHT)),
        (Position(3), Piece(color=Color.WHITE, type=PieceType.BISHOP)),
        (Position(4), Piece(color=Color.WHITE, type=PieceType.QUEEN)),
        (Position(5), Piece(color=Color.WHITE, type=PieceType.KING)),
        (Position(6), Piece(color=Color.WHITE, type=PieceType.BISHOP)),
        (Position(7), Piece(color=Color.WHITE, type=PieceType.KNIGHT)),
        (Position(8), Piece(color=Color.WHITE, type=PieceType.ROOK)),

        (Position(9), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(10), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(11), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(12), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(13), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(14), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(15), Piece(color=Color.WHITE, type=PieceType.PAWN)),
        (Position(16), Piece(color=Color.WHITE, type=PieceType.PAWN)),

        (Position(49), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(50), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(51), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(52), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(53), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(54), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(55), Piece(color=Color.BLACK, type=PieceType.PAWN)),
        (Position(56), Piece(color=Color.BLACK, type=PieceType.PAWN)),

        (Position(57), Piece(color=Color.BLACK, type=PieceType.ROOK)),
        (Position(58), Piece(color=Color.BLACK, type=PieceType.KNIGHT)),
        (Position(59), Piece(color=Color.BLACK, type=PieceType.BISHOP)),
        (Position(60), Piece(color=Color.BLACK, type=PieceType.QUEEN)),
        (Position(61), Piece(color=Color.BLACK, type=PieceType.KING)),
        (Position(62), Piece(color=Color.BLACK, type=PieceType.BISHOP)),
        (Position(63), Piece(color=Color.BLACK, type=PieceType.KNIGHT)),
        (Position(64), Piece(color=Color.BLACK, type=PieceType.ROOK)),

    ] + [(Position(x), None) for x in range(17, 49)]
    state = dict(state)

    for move in board.moves:
        if move.move_type == MoveType.REGULAR:
            state[move.target] = state[move.source]
            state[move.source] = None
        elif move.move_type == MoveType.CASTLE_LONG:
            if move.color == Color.WHITE:
                state[Position(5)] = None
                state[Position(1)] = None
                state[Position(3)] = Piece(color=Color.WHITE, type=PieceType.KING)
                state[Position(4)] = Piece(color=Color.WHITE, type=PieceType.ROOK)
            else:
                state[Position(61)] = None
                state[Position(57)] = None
                state[Position(59)] = Piece(color=Color.WHITE, type=PieceType.KING)
                state[Position(60)] = Piece(color=Color.WHITE, type=PieceType.ROOK)
        elif move.move_type == MoveType.CASTLE_SHORT:
            if move.color == Color.WHITE:
                state[Position(5)] = None
                state[Position(8)] = None
                state[Position(7)] = Piece(color=Color.WHITE, type=PieceType.KING)
                state[Position(6)] = Piece(color=Color.WHITE, type=PieceType.ROOK)
            else:
                state[Position(61)] = None
                state[Position(64)] = None
                state[Position(63)] = Piece(color=Color.WHITE, type=PieceType.KING)
                state[Position(62)] = Piece(color=Color.WHITE, type=PieceType.ROOK)
        elif move.move_type == MoveType.PROMOTION_BISHOP:
            state[Position(move.source)] = Piece(color=move.color, type=PieceType.BISHOP)
        elif move.move_type == MoveType.PROMOTION_QUEEN:
            state[Position(move.source)] = Piece(color=move.color, type=PieceType.QUEEN)
        elif move.move_type == MoveType.PROMOTION_KNIGHT:
            state[Position(move.source)] = Piece(color=move.color, type=PieceType.KNIGHT)
        elif move.move_type == MoveType.PROMOTION_ROOK:
            state[Position(move.source)] = Piece(color=move.color, type=PieceType.ROOK)

    board.state = OrderedDict(sorted(state.items()))
    return board
