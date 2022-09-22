//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChessPiece.h"

ChessPiece::ChessPiece(ChessPieceSide side, ChessPieceType type) {
    this->color = side;
    this->piece = type;
}

ChessPiece::ChessPiece(std::string side, std::string type) {
    auto pside = side == "white" ? ChessPieceSide::WHITE : ChessPieceSide::BLACK;
    this->color = pside;
    if (type == "king") {
        this->piece = ChessPieceType::KING;
    } else if (type == "rook") {
        this->piece = ChessPieceType::ROOK;
    } else if (type == "bishop") {
        this->piece = ChessPieceType::BISHOP;
    } else if (type == "queen") {
        this->piece = ChessPieceType::QUEEN;
    } else if (type == "knight") {
        this->piece = ChessPieceType::KNIGHT;
    } else if (type == "pawn") {
        this->piece = ChessPieceType::PAWN;
    } else {
        this->piece = ChessPieceType::KING;
    }
}

ChessPieceType ChessPiece::get_type() {
    return this->piece;
}

ChessPieceSide ChessPiece::get_side() {
    return this->color;
}

std::string ChessPiece::get_name() {
    switch (piece) {
        case ChessPieceType::KING:
            return "K";
        case ChessPieceType::ROOK:
            return "R";
        case ChessPieceType::BISHOP:
            return "B";
        case ChessPieceType::QUEEN:
            return "Q";
        case ChessPieceType::KNIGHT:
            return "Kn";
        case ChessPieceType::PAWN:
            return "P";
    }
}

std::vector<Position> ChessPiece::get_regular_moves(Position position, ChessPiece piece) {
    int xcor = position.getXCoordinate();
    int ycor = position.getYCoordinate();
    int ytop = ycor + 1;
    int ydown = ycor - 1;
    std::vector<Position> result;
    switch (piece.get_type()) {
        case ChessPieceType::KING:
            for (int x = xcor-1; x <= xcor+1; x++) {
                for (int y = ycor-1; y <= ycor+1; y++) {
                    if (x != xcor || y != ycor) {
                        result.emplace_back(x, y);
                    }
                }
            }
            return remove_invalid_moves(result);
        case ChessPieceType::QUEEN:
            for (int x = xcor+1; x <= 7; x++) {
                if (ytop <= 7) {
                    result.emplace_back(x, ytop);
                }
                result.emplace_back(x, ycor);
                if (ydown >= 0) {
                    result.emplace_back(x, ydown);
                }
                ytop++;
                ydown--;
            }
            ytop = ycor + 1;
            ydown = ycor - 1;
            for (int x = xcor-1; x >= 0; x--) {
                if (ytop <= 7) {
                    result.emplace_back(x, ytop);
                }
                result.emplace_back(x, ycor);
                if (ydown >= 0) {
                    result.emplace_back(x, ydown);
                }
                ytop++;
                ydown--;
            }
            for (int y = 0; y <= 7; y++) {
                if (y != ycor) {
                    result.emplace_back(xcor, y);
                }
            }
            return remove_invalid_moves(result);
        case ChessPieceType::ROOK:
            for (int x = 0; x <= 7; x++) {
                if (x != xcor) {
                    result.emplace_back(x, ycor);
                }
            }
            for (int y = 0; y <= 7; y++) {
                if (y != ycor) {
                    result.emplace_back(xcor, y);
                }
            }
            return remove_invalid_moves(result);
        case ChessPieceType::BISHOP:
            for (int x = xcor+1; x <= 7 && (ytop <= 7 || ydown >= 0); x++) {
                if (ytop <= 7) {
                    result.emplace_back(x, ytop);
                }
                if (ydown >= 0) {
                    result.emplace_back(x, ydown);
                }
                ytop++;
                ydown--;
            }
            ytop = ycor + 1;
            ydown = ycor - 1;
            for (int x = xcor-1; x >= 0 && (ytop <= 7 || ydown >= 0); x--) {
                if (ytop <= 7) {
                    result.emplace_back(x, ytop);
                }
                if (ydown >= 0) {
                    result.emplace_back(x, ydown);
                }
                ytop++;
                ydown--;
            }
            return remove_invalid_moves(result);
        case ChessPieceType::KNIGHT:
            for (int x = xcor - 2; x <= xcor + 2; x += 4) {
                result.emplace_back(x, ytop);
                result.emplace_back(x, ydown);
            }
            for (int x = xcor - 1; x <= xcor + 1; x += 2) {
                result.emplace_back(x, ytop + 1);
                result.emplace_back(x, ydown - 1);
            }
            return remove_invalid_moves(result);
        case ChessPieceType::PAWN:
            // Attack moves
            result.emplace_back(xcor + 1, piece.get_side() == ChessPieceSide::WHITE ? ycor + 1 : ycor - 1);
            result.emplace_back(xcor - 1, piece.get_side() == ChessPieceSide::WHITE ? ycor + 1 : ycor - 1);
            // Double move
            if (ycor == (piece.get_side() == ChessPieceSide::WHITE ? 1 : 6)) {
                result.emplace_back(xcor, piece.get_side() == ChessPieceSide::WHITE ? ycor + 2 : ycor - 2);
            }
            // Forward move
            result.emplace_back(xcor, piece.get_side() == ChessPieceSide::WHITE ? ycor + 1 : ycor - 1);
            return remove_invalid_moves(result);
    }
}

bool ChessPiece::operator==(const ChessPiece &other) const {
    return this->piece == other.piece && this->color == other.color;
}

std::vector<Position> ChessPiece::remove_invalid_moves(std::vector<Position> positions) {
    for (int i = 0; i < positions.size(); i++) {
        if (positions.at(i).getPosition() < 0 || positions.at(i).getPosition() > 63) {
            positions.erase(positions.cbegin() + i);
            i--;
        }
    }
    return positions;
}

bool ChessPiece::operator!=(const ChessPiece &other) const {
    return piece != other.piece || color != other.color;
}


