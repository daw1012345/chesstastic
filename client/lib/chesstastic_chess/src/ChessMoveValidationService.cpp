//
// Created by degra on 01/11/2021.
//

#include <algorithm>
#include "ChessMoveValidationService.h"

bool ChessMoveValidationService::isValidMove(ChessMove move, ChessBoard board) {
    switch (move.getType()) {
        case MoveType::REGULAR: {
            if (isValidRegularMove(move, board)) {
                break;
            }
            return false;
        }
        case MoveType::CASTLE_LONG: {
            if (isValidCastleLong(move.getPiece().get_side(), board)) {
                break;
            }
            return false;
        }
        case MoveType::CASTLE_SHORT:
            if (isValidCastleShort(move.getPiece().get_side(), board)) {
                break;
            }
            return false;
        case MoveType::PROMOTION_BISHOP:
        case MoveType::PROMOTION_KNIGHT:
        case MoveType::PROMOTION_ROOK:
        case MoveType::PROMOTION_QUEEN: {
            if (isValidPromotion(move, board)) {
                break;
            }
            return false;
        }
        case MoveType::RESIGNATION: {
            return true;
        }
    }
    // If valid move otherwise, we check if making the move will put player's own king in check, which would make the
    // move illegal.
    ChessBoard tempBoard = ChessBoard::make_move(board, move);
    std::vector<Position> kingPosition = {tempBoard.get_location_of(ChessPiece(move.getPiece().get_side(),
                                                                               ChessPieceType::KING)).front()};
    return !aPositionUnderAttack(kingPosition, move.getPiece().get_side() == ChessPieceSide::WHITE ?
                                             ChessPieceSide::BLACK : ChessPieceSide::WHITE, tempBoard);
}

// Check for regular move, one position forwards, no pieces must be on destination
bool ChessMoveValidationService::isValidForwardMovePawn(ChessMove move, Position dest, Position source, ChessBoard board) {
    bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
    return board.is_field_empty(dest) && dest.getXCoordinate() == source.getXCoordinate() &&
    dest.getYCoordinate() == (isWhite ? source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
}

// Check for attack move, right or left, opponents piece must be on destination
bool ChessMoveValidationService::isValidAttackMovePawn(ChessMove move, Position dest, Position source,
                                                       std::optional<ChessPiece> destinationPiece) {
    bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
    Position moveLeft = Position(source.getXCoordinate() - 1, isWhite ?
        source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
    Position moveRight =Position(source.getXCoordinate() + 1, isWhite ?
        source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
    return destinationPiece.has_value() &&
           ((moveLeft == dest &&
                   destinationPiece->get_side() != move.getPiece().get_side()) ||
            (moveRight == dest &&
                    destinationPiece->get_side() != move.getPiece().get_side()));
}

// Double move, check if position 1 and 2 above source is empty and if source is at starting row.
bool ChessMoveValidationService::isValidDoubleMovePawn(Position dest, Position source, ChessMove move, ChessBoard board) {
    bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
    Position regularMove = Position(source.getXCoordinate(),
                                    isWhite ? source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
    Position doubleMove = Position(source.getXCoordinate(),
                                   isWhite ? source.getYCoordinate() + 2 : source.getYCoordinate() - 2);
    int startingRow = isWhite ? 1 : 6;
    return dest == doubleMove &&
            board.is_field_empty(doubleMove) &&
           board.is_field_empty(regularMove) &&
           source.getYCoordinate() == startingRow;
}

// En Passant, check if destination is one up and one left/right, if there is an opponents pawn directly to left or right
// of source position and if in correct row.
bool ChessMoveValidationService::isValidEnPassantMovePawn(ChessMove move, Position source, Position dest, ChessBoard board) {
    bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
    int enPassantRow = isWhite ? 4 : 3;
    Position positionLeft = Position(source.getXCoordinate() - 1,
                                 isWhite ? source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
    Position positionRight = Position(source.getXCoordinate() + 1,
                                  isWhite ? source.getYCoordinate() + 1 : source.getYCoordinate() - 1);
    if (board.get_field(Position(dest.getXCoordinate(), enPassantRow)).has_value()) {
        ChessPiece opponentPiece = board.get_field(Position(dest.getXCoordinate(), enPassantRow)).value();
        return board.is_field_empty(dest) && source.getYCoordinate() == enPassantRow &&
               (dest == positionLeft || dest == positionRight) && opponentPiece.get_side() != move.getPiece().get_side() &&
                opponentPiece.get_type() == ChessPieceType::PAWN &&
               board.is_field_empty(Position(dest.getXCoordinate(), isWhite ? 6 : 1));
    }
    return false;
}

bool ChessMoveValidationService::isValidRegularMove(ChessMove move, ChessBoard board) {
    Position dest = move.getDestination();
    Position source = move.getSource();
    std::optional<ChessPiece> sourcePiece = board.get_field(source);
    std::optional<ChessPiece> destinationPiece = board.get_field(dest);
    if (board.is_field_empty(source) || sourcePiece.value() != move.getPiece()) {
        return false;
    }
    std::vector<Position> positions;
    if (move.getPiece().get_type() != ChessPieceType::PAWN) {
        positions = ChessPiece::get_regular_moves(move.getSource(), move.getPiece());
    }
    switch (move.getPiece().get_type()) {
        // For en passant, does not check if previous move by opponent was double move since no history is available,
        // this needs to be checked server side.
        case (ChessPieceType::PAWN): {
            return (isValidForwardMovePawn(move, dest, source, board) ||
                    isValidAttackMovePawn(move, dest, source, destinationPiece) ||
                    isValidDoubleMovePawn(dest, source, move, board) ||
                    isValidEnPassantMovePawn(move, source, dest, board));
        }
        case (ChessPieceType::KNIGHT):
        case (ChessPieceType::KING): {
            if (std::count(positions.begin(), positions.end(), dest) && (board.is_field_empty(dest) ||
                    destinationPiece->get_side() != move.getPiece().get_side())) {
                return true;
            }
            return false;
        }
        case (ChessPieceType::BISHOP): {
            if (std::count(positions.begin(), positions.end(), dest) != 0 &&
                    (board.is_field_empty(dest) || destinationPiece->get_side() != sourcePiece->get_side())) {
                std::vector<Position> blockingPositions;
                int y = source.getYCoordinate();
                if (dest.getYCoordinate() > source.getYCoordinate() && dest.getXCoordinate() > source.getXCoordinate()) {
                    for (int x = source.getXCoordinate() + 1; x < dest.getXCoordinate(); x++) {
                        y++;
                        blockingPositions.emplace_back(x, y);
                    }
                } else if (dest.getYCoordinate() > source.getYCoordinate() && dest.getXCoordinate() < source.getXCoordinate()) {
                    for (int x = source.getXCoordinate() - 1; x > dest.getXCoordinate(); x--) {
                        y++;
                        blockingPositions.emplace_back(x, y);
                    }
                } else if (dest.getYCoordinate() < source.getYCoordinate() && dest.getXCoordinate() > source.getXCoordinate()) {
                    for (int x = source.getXCoordinate() + 1; x < dest.getXCoordinate(); x++) {
                        y--;
                        blockingPositions.emplace_back(x, y);
                    }
                } else if (dest.getYCoordinate() < source.getYCoordinate() && dest.getXCoordinate() < source.getXCoordinate()) {
                    for (int x = source.getXCoordinate() - 1; x > dest.getXCoordinate(); x--) {
                        y--;
                        blockingPositions.emplace_back(x, y);
                    }
                }
                return emptySquares(blockingPositions, board);
            }
            return false;
        }
        case (ChessPieceType::ROOK): {
            if (std::count(positions.begin(), positions.end(), dest) != 0 && (board.is_field_empty(dest) ||
                    destinationPiece->get_side() !=
                    move.getPiece().get_side())) {
                std::vector<Position> blockingPositions;
                if (dest.getYCoordinate() > source.getYCoordinate()) {
                    for (int i = source.getYCoordinate() + 1; i < dest.getYCoordinate(); i++) {
                        blockingPositions.emplace_back(dest.getXCoordinate(), i);
                    }
                } else if (dest.getYCoordinate() < source.getYCoordinate()) {
                    for (int i = source.getYCoordinate() - 1; i > dest.getYCoordinate(); i--) {
                        blockingPositions.emplace_back(dest.getXCoordinate(), i);
                    }
                } else if (dest.getXCoordinate() > source.getXCoordinate()) {
                    for (int i = source.getXCoordinate() + 1; i < dest.getXCoordinate(); i++) {
                        blockingPositions.emplace_back(i, dest.getYCoordinate());
                    }
                } else if (dest.getXCoordinate() < source.getXCoordinate()) {
                    for (int i = source.getXCoordinate() - 1; i > dest.getXCoordinate(); i--) {
                        blockingPositions.emplace_back(i, dest.getYCoordinate());
                    }
                }
                return emptySquares(blockingPositions, board);
            }
            return false;
        }
        case (ChessPieceType::QUEEN): {
            if (source.getXCoordinate() != dest.getXCoordinate() && source.getYCoordinate() != dest.getYCoordinate()) {
                if (std::count(positions.begin(), positions.end(), dest) != 0 &&
                    (board.is_field_empty(dest) || destinationPiece->get_side() != sourcePiece->get_side())) {
                    std::vector<Position> blockingPositions;
                    int y = source.getYCoordinate();
                    if (dest.getYCoordinate() > source.getYCoordinate() &&
                        dest.getXCoordinate() > source.getXCoordinate()) {
                        for (int x = source.getXCoordinate() + 1; x < dest.getXCoordinate(); x++) {
                            y++;
                            blockingPositions.emplace_back(x, y);
                        }
                    } else if (dest.getYCoordinate() > source.getYCoordinate() &&
                               dest.getXCoordinate() < source.getXCoordinate()) {
                        for (int x = source.getXCoordinate() - 1; x > dest.getXCoordinate(); x--) {
                            y++;
                            blockingPositions.emplace_back(x, y);
                        }
                    } else if (dest.getYCoordinate() < source.getYCoordinate() &&
                               dest.getXCoordinate() > source.getXCoordinate()) {
                        for (int x = source.getXCoordinate() + 1; x < dest.getXCoordinate(); x++) {
                            y--;
                            blockingPositions.emplace_back(x, y);
                        }
                    } else if (dest.getYCoordinate() < source.getYCoordinate() &&
                               dest.getXCoordinate() < source.getXCoordinate()) {
                        for (int x = source.getXCoordinate() - 1; x > dest.getXCoordinate(); x--) {
                            y--;
                            blockingPositions.emplace_back(x, y);
                        }
                    }
                    return emptySquares(blockingPositions, board);
                }
            } else {
                if (std::count(positions.begin(), positions.end(), dest) != 0 && (board.is_field_empty(dest) ||
                        destinationPiece->get_side() !=
                        move.getPiece().get_side())) {
                    std::vector<Position> blockingPositions;
                    if (dest.getYCoordinate() > source.getYCoordinate()) {
                        for (int i = source.getYCoordinate() + 1; i < dest.getYCoordinate(); i++) {
                            blockingPositions.emplace_back(dest.getXCoordinate(), i);
                        }
                    } else if (dest.getYCoordinate() < source.getYCoordinate()) {
                        for (int i = source.getYCoordinate() - 1; i > dest.getYCoordinate(); i--) {
                            blockingPositions.emplace_back(dest.getXCoordinate(), i);
                        }
                    } else if (dest.getXCoordinate() > source.getXCoordinate()) {
                        for (int i = source.getXCoordinate() + 1; i < dest.getXCoordinate(); i++) {
                            blockingPositions.emplace_back(i, dest.getYCoordinate());
                        }
                    } else if (dest.getXCoordinate() < source.getXCoordinate()) {
                        for (int i = source.getXCoordinate() - 1; i > dest.getYCoordinate(); i--) {
                            blockingPositions.emplace_back(i, dest.getYCoordinate());
                        }
                    }
                    return emptySquares(blockingPositions, board);
                }
            }
            return false;
        }
        default: {
            return false;
        }
    }
}

/*
 * Check if rook and king are in correct position, if king is not in check and if nothing in between, we don't check
 * if king and/or rook have moved before, this should be a serverside check.
 */
bool ChessMoveValidationService::isValidCastleLong(ChessPieceSide side, ChessBoard board) {
    std::vector<Position> positions;
    if (side == ChessPieceSide::WHITE) {
        if (board.is_field_empty(Position(4)) ||
                board.get_field(Position(4))->get_type() != ChessPieceType::KING ||
            board.is_field_empty(Position(0)) ||
                board.get_field(Position(0))->get_type() != ChessPieceType::ROOK) {
            return false;
        }
        positions.emplace_back(1);
        positions.emplace_back(2);
        positions.emplace_back(3);
        if (!emptySquares(positions, board)) {
            return false;
        }
        positions.clear();
        positions.emplace_back(2);
        positions.emplace_back(3);
        positions.emplace_back(4);
        if (aPositionUnderAttack(positions, ChessPieceSide::BLACK, board)) {
            return false;
        }
    } else {
        if (board.is_field_empty(Position(60)) ||
                board.get_field(Position(60))->get_type() != ChessPieceType::KING ||
            board.is_field_empty(Position(56)) ||
                board.get_field(Position(56))->get_type() != ChessPieceType::ROOK) {
            return false;
        }
        positions.emplace_back(57);
        positions.emplace_back(58);
        positions.emplace_back(59);
        if (!emptySquares(positions, board)) {
            return false;
        }
        positions.clear();
        positions.emplace_back(58);
        positions.emplace_back(59);
        positions.emplace_back(60);
        if (aPositionUnderAttack(positions, ChessPieceSide::WHITE, board)) {
            return false;
        }
    }
    return true;
}

bool ChessMoveValidationService::isValidCastleShort(ChessPieceSide side, ChessBoard board) {
    std::vector<Position> positions;
    if (side == ChessPieceSide::WHITE) {
        if (board.is_field_empty(Position(4)) ||
                board.get_field(Position(4))->get_type() != ChessPieceType::KING ||
            board.is_field_empty(Position(7)) ||
                board.get_field(Position(7))->get_type() != ChessPieceType::ROOK) {
            return false;
        }
        positions.emplace_back(5);
        positions.emplace_back(6);
        if (!emptySquares(positions, board)) {
            return false;
        }
        positions.emplace_back(4);
        if (aPositionUnderAttack(positions, ChessPieceSide::BLACK, board)) {
            return false;
        }
    } else {
        if (board.is_field_empty(Position(60)) ||
                board.get_field(Position(60))->get_type() != ChessPieceType::KING ||
            board.is_field_empty(Position(63)) ||
                board.get_field(Position(63))->get_type() != ChessPieceType::ROOK) {
            return false;
        }
        positions.emplace_back(61);
        positions.emplace_back(62);
        if (!emptySquares(positions, board)) {
            return false;
        }
        positions.emplace_back(60);
        if (aPositionUnderAttack(positions, ChessPieceSide::WHITE, board)) {
            return false;
        }
    }
    return true;
}

bool ChessMoveValidationService::isValidPromotion(ChessMove move, ChessBoard board) {
    if (move.getPiece().get_type() == ChessPieceType::PAWN) {
        if (move.getPiece().get_side() == ChessPieceSide::WHITE &&
                move.getSource().getYCoordinate() == 6 &&
                move.getDestination().getYCoordinate() == 7 &&
            isValidRegularMove(move, board)) {
            return true;
        }
        if (move.getPiece().get_side() == ChessPieceSide::BLACK &&
                move.getSource().getYCoordinate() == 1 &&
                move.getDestination().getYCoordinate() == 0 &&
            isValidRegularMove(move, board)) {
            return true;
        }
        return false;
    }
}

std::vector<ChessMove> ChessMoveValidationService::getPossibleMoves(ChessPiece piece, Position position, ChessBoard board) {
    std::vector<ChessMove> result;
    std::vector<Position> positions = piece.get_regular_moves(position, piece);
    for (Position pos : positions) {
        ChessMove move = ChessMove(piece, position, pos, MoveType::REGULAR);
        if (isValidMove(move, board)) {
            if ((move.getSource().getYCoordinate() == (piece.get_side() == ChessPieceSide::WHITE ? 6 : 1)) &&
                (piece.get_type() == ChessPieceType::PAWN)) {
                result.emplace_back(piece, position, pos, MoveType::PROMOTION_BISHOP);
                result.emplace_back(piece, position, pos, MoveType::PROMOTION_KNIGHT);
                result.emplace_back(piece, position, pos, MoveType::PROMOTION_ROOK);
                result.emplace_back(piece, position, pos, MoveType::PROMOTION_QUEEN);
            } else {
                result.push_back(move);
            }
        }
    }
    if (piece.get_type() == ChessPieceType::KING) {
        if (isValidCastleLong(piece.get_side(), board)) {
            result.emplace_back(piece, 0, 0, MoveType::CASTLE_LONG);
        }
        if (isValidCastleShort(piece.get_side(), board)) {
            result.emplace_back(piece, 0, 0, MoveType::CASTLE_SHORT);
        }
    }
    return result;
}

bool ChessMoveValidationService::emptySquares(std::vector<Position> positions, ChessBoard board) {
    for (Position pos : positions) {
        if (board.get_field(pos).has_value()) {
            return false;
        }
    }
    return true;
}

bool ChessMoveValidationService::aPositionUnderAttack(std::vector<Position> positions, ChessPieceSide attacker, ChessBoard board) {
    for (Position pos : positions) {
        ChessBoard temp = board;
        if (board.is_field_empty(pos) || board.get_field(pos).value().get_side() == attacker) {
            temp = ChessBoard(board.get_board());
            temp.set_field(pos, ChessPiece(ChessPieceSide::WHITE == attacker ?
                                           ChessPieceSide::BLACK : ChessPieceSide::WHITE, ChessPieceType::PAWN));
        }
        for (std::pair<Position, ChessPiece> pair : board.get_board()) {
            ChessMove possibleMove = ChessMove(pair.second, pair.first, pos, MoveType::REGULAR);
            if (pair.second.get_side() == attacker) {
                if (isValidRegularMove(possibleMove, temp)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessMoveValidationService::allPositionsUnderAttack(std::vector<Position> positions, ChessPieceSide attacker, ChessBoard board) {
    for (Position pos : positions) {
        std::vector<Position> position;
        position.push_back(pos);
        if (!aPositionUnderAttack(position, attacker, board)) {
            return false;
        }
    }
    return true;
}
