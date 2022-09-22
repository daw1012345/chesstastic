//
// Created by degra on 01/11/2021.
//

#include "Position.h"

Position::Position(int position) {
    this->position = position;
}

Position::Position(int x, int y) {
    setCoordinates(x, y);
}

int Position::getPosition() const {
    return position;
}

void Position::setPosition(int position) {
    this->position = position;
}

int Position::getXCoordinate() {
    return position % 8;
}

int Position::getYCoordinate() {
    return position / 8;
}

void Position::setCoordinates(int x, int y) {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        this->position=-1;
    } else {
        this->position = y * 8 + x;
    }
}

bool Position::operator==(Position position2) const {
    return this->getPosition() == position2.getPosition();
}

bool Position::operator<(Position position2) const {
    return this->getPosition() < position2.getPosition();
}

bool Position::operator>(Position position2) const {
    return this->getPosition() > position2.getPosition();
}

bool Position::operator!=(Position position2) const {
    return this->getPosition() != position2.getPosition();
}


