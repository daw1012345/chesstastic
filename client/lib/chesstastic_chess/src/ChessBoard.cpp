//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include <vector>
#include "ChessBoard.h"

#include <utility>

ChessBoard::ChessBoard(std::map<Position, ChessPiece> init_board) {
    this->board = init_board;
}

bool ChessBoard::is_board_empty() const {
    return this->board.empty();
}

bool ChessBoard::is_field_empty(Position position) const {
    return this->board.count(position) == 0;
}

void ChessBoard::set_field(Position position, ChessPiece piece) {
    this->board.insert_or_assign(position, piece);
}

std::optional<ChessPiece> ChessBoard::get_field(Position position) const {
    if (is_field_empty(position)) return std::nullopt;
    return this->board.at(position);
}

std::map<Position, ChessPiece> ChessBoard::get_board() const {
    return this->board;
}

std::vector<Position> ChessBoard::get_location_of(ChessPiece piece) {
    std::vector<Position> result;
    for (auto&[key, value]: this->board) {
        if (value == piece) {
            result.push_back(key);
        }
    }

    return result;
}

std::optional<ChessPiece> ChessBoard::remove_at(Position position) {
    if (this->is_field_empty(position)) return std::nullopt;
    auto old = this->board.at(position);
    this->board.erase(position);
    return old;
}

std::map<ChessPieceType, int> ChessBoard::get_missing_pieces(ChessPieceSide color) {
    const int EXPECTED_PAWN = 8;
    const int EXPECTED_ROYALTY = 1;
    const int EXPECTED_DEFAULT = 2; // Most pieces are duplicated

    const auto STD_TO_ITER = {ChessPieceType::BISHOP, ChessPieceType::KNIGHT, ChessPieceType::ROOK};

    std::map<ChessPieceType, int> result;

    int missing_pawns = EXPECTED_PAWN - this->get_location_of({color, ChessPieceType::PAWN}).size();
    int missing_king = EXPECTED_ROYALTY - this->get_location_of({color, ChessPieceType::KING}).size();
    int missing_queen = EXPECTED_ROYALTY - this->get_location_of({color, ChessPieceType::QUEEN}).size();

    result.insert({ChessPieceType::PAWN, missing_pawns});
    result.insert({ChessPieceType::KING, missing_king});
    result.insert({ChessPieceType::QUEEN, missing_queen});

    for (auto piece: STD_TO_ITER) {
        int missing_n = EXPECTED_DEFAULT - this->get_location_of({color, piece}).size();
        result.insert({piece, missing_n});
    }

    return result;

}
std::map<Position, ChessPiece> ChessBoard::make_move(ChessBoard board, ChessMove move) {
    ChessBoard tempBoard = ChessBoard(board);
    tempBoard.make_move(move);
    return tempBoard.get_board();
}

void ChessBoard::make_move(ChessMove move) {
    switch (move.getType()) {
        case MoveType::REGULAR: {
            do_move(move, move.getPiece().get_type());
            break;
        }
        case MoveType::PROMOTION_QUEEN: {
            do_move(move, ChessPieceType::QUEEN);
            break;
        }
        case MoveType::PROMOTION_ROOK: {
            do_move(move, ChessPieceType::ROOK);
            break;
        }
        case MoveType::PROMOTION_KNIGHT: {
            do_move(move, ChessPieceType::KNIGHT);
            break;
        }
        case MoveType::PROMOTION_BISHOP: {
            do_move(move, ChessPieceType::BISHOP);
            break;
        }
        case MoveType::CASTLE_SHORT: {
            bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
            remove_at(Position(isWhite ? 4 : 60));
            remove_at(Position(isWhite ? 7 : 63));
            set_field(Position(isWhite ? 6 : 62),
                      ChessPiece(isWhite ? ChessPieceSide::WHITE : ChessPieceSide::BLACK, ChessPieceType::KING));
            set_field(Position(isWhite ? 5 : 61),
                      ChessPiece(isWhite ? ChessPieceSide::WHITE : ChessPieceSide::BLACK, ChessPieceType::ROOK));
            break;
        }
        case MoveType::CASTLE_LONG: {
            bool isWhite = move.getPiece().get_side() == ChessPieceSide::WHITE;
            remove_at(Position(isWhite ? 4 : 60));
            remove_at(Position(isWhite ? 0 : 56));
            set_field(Position(isWhite ? 2 : 58),
                      ChessPiece(isWhite ? ChessPieceSide::WHITE : ChessPieceSide::BLACK, ChessPieceType::KING));
            set_field(Position(isWhite ? 3 : 9),
                      ChessPiece(isWhite ? ChessPieceSide::WHITE : ChessPieceSide::BLACK, ChessPieceType::ROOK));
            break;
        }
        case MoveType::RESIGNATION: {

        }
    }
}

void ChessBoard::do_move(ChessMove move, ChessPieceType newType) {
    remove_at(move.getSource());
    if (move.getPiece().get_type() == ChessPieceType::PAWN && move.getSource().getXCoordinate() !=
                                                              move.getDestination().getXCoordinate() && is_field_empty(move.getDestination())) {
        remove_at(Position(move.getDestination().getXCoordinate(), move.getPiece().get_side() ==
                                                                   ChessPieceSide::WHITE ?
                                                                   move.getDestination().getYCoordinate() - 1 :
                                                                   move.getDestination().getYCoordinate() + 1));
    } else {
        remove_at(move.getDestination());
    }
    set_field(move.getDestination(), ChessPiece(move.getPiece().get_side(), newType));
}
