//
// Created by degra on 01/11/2021.
//

#include "ChessMove.h"

ChessMove::ChessMove(ChessPiece piece, Position source, Position destination, MoveType type)
                            : source(source), piece(piece), destination(destination), type(type){}

void ChessMove::setSource(Position source) {
    this->source = source;
}

Position ChessMove::getSource() const {
    return source;
}

Position ChessMove::getDestination() const {
    return destination;
}

void ChessMove::setDestination(Position destination) {
    this->destination = destination;
}

MoveType ChessMove::getType() const {
    return type;
}

void ChessMove::setType(MoveType type) {
    this->type = type;
}

ChessPiece ChessMove::getPiece() const {
    return piece;
}

bool ChessMove::operator==(ChessMove move2) const {
    return this->getPiece() == move2.getPiece() && this->getDestination() == move2.getDestination() &&
    this->getSource() == move2.getSource() && this->getType() == move2.getType();
}

bool ChessMove::operator!=(ChessMove move2) const {
    return this->getPiece() != move2.getPiece() || this->getDestination() != move2.getDestination() ||
           this->getSource() != move2.getSource() || this->getType() != move2.getType();
}

bool ChessMove::operator<(ChessMove move2) const {
    if (this->getSource() < move2.getSource()) {
        return true;
    } else if (this->getSource() == move2.getSource() && this->getDestination() < move2.getDestination()) {
        return true;
    } else if (this->getSource() == move2.getSource() && this->getDestination() == move2.getDestination() &&
                this->getType() != move2.getType()) {
        return getType() == MoveType::PROMOTION_BISHOP ||
                (getType() == MoveType::PROMOTION_KNIGHT  && move2.getType() != MoveType::PROMOTION_BISHOP) ||
                (getType() == MoveType::PROMOTION_ROOK && move2.getType() == MoveType::PROMOTION_QUEEN) ||
                getType() == MoveType::CASTLE_SHORT;
    } else {
        return false;
    }
}

bool ChessMove::operator>(ChessMove move2) const {
    if (this->getSource() > move2.getSource()) {
        return true;
    } else if (this->getSource() == move2.getSource() && this->getDestination() > move2.getDestination()) {
        return true;
    } else if (this->getSource() == move2.getSource() && this->getDestination() == move2.getDestination() &&
               this->getType() != move2.getType()){
        return getType() == MoveType::PROMOTION_QUEEN ||
               (getType() == MoveType::PROMOTION_ROOK  && move2.getType() != MoveType::PROMOTION_QUEEN) ||
               (getType() == MoveType::PROMOTION_KNIGHT && move2.getType() == MoveType::PROMOTION_BISHOP) ||
               getType() == MoveType::CASTLE_LONG;
    }
}
