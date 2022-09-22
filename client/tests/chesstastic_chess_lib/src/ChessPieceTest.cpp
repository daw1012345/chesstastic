//
// Created by degra on 04/11/2021.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <chesstastic_chess/ChessPiece.h>
#include <string>
#include <iostream>

// Test for constructor ChessPiece(String, String).
TEST(ChessPieceTest, StringConstructor) {
    ChessPiece spiece1 = ChessPiece("white", "knight");
    ChessPiece piece1 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT);
    ChessPiece spiece2 = ChessPiece("black", "pawn");
    ChessPiece piece2 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN);
    ChessPiece spiece3 = ChessPiece("white", "king");
    ChessPiece piece3 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING);
    ChessPiece spiece4 = ChessPiece("black", "queen");
    ChessPiece piece4 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::QUEEN);
    ChessPiece spiece5 = ChessPiece("white", "bishop");
    ChessPiece piece5 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP);
    ChessPiece spiece6 = ChessPiece("black", "rook");
    ChessPiece piece6 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK);
    EXPECT_EQ(spiece1, piece1);
    EXPECT_EQ(spiece2, piece2);
    EXPECT_EQ(spiece3, piece3);
    EXPECT_EQ(spiece4, piece4);
    EXPECT_EQ(spiece5, piece5);
    EXPECT_EQ(spiece6, piece6);
}

// Test for getType(), check if correct ChessPieceType is returned.
TEST(ChessPieceTest, getType) {
    ChessPiece knight = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT);
    ChessPiece rook = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK);
    ChessPiece pawn = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN);
    EXPECT_EQ(ChessPieceType::KNIGHT, knight.get_type());
    EXPECT_EQ(ChessPieceType::ROOK, rook.get_type());
    EXPECT_EQ(ChessPieceType::PAWN, pawn.get_type());
}

// Test for getSide(), check if correct ChessPieceSide is returned.
TEST(ChessPieceTest, getSide) {
    ChessPiece black1 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT);
    ChessPiece white = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK);
    ChessPiece black2 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN);
    EXPECT_EQ(ChessPieceSide::BLACK, black1.get_side());
    EXPECT_EQ(ChessPieceSide::WHITE, white.get_side());
    EXPECT_EQ(ChessPieceSide::BLACK, black2.get_side());
}

// Test for getName(), check if correct name is returned.
TEST(ChessPieceTest, getName) {
    EXPECT_EQ("Kn", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT).get_name());
    EXPECT_EQ("R", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK).get_name());
    EXPECT_EQ("B", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::BISHOP).get_name());
    EXPECT_EQ("K", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING).get_name());
    EXPECT_EQ("Q", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::QUEEN).get_name());
    EXPECT_EQ("P", ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN).get_name());
}

// Test for operator==(ChessPiece), check if two instances of ChessPiece are equal to each other when both side and type are the same.
TEST(ChessPieceTest, OperatorEquals) {
    ChessPiece piece1 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN);
    ChessPiece piece2 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN);
    ChessPiece piece3 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN);
    ChessPiece piece4 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT);
    EXPECT_TRUE(piece1 == piece2);
    EXPECT_FALSE(piece1 == piece3);
    EXPECT_FALSE(piece1 == piece4);
}

// Test for operator!=(ChessPiece), check if two instances of ChessPiece are not equal to each other if either side or type are different.
TEST(ChessPieceTest, OperatorNotEquals) {
    ChessPiece piece1 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN);
    ChessPiece piece2 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN);
    ChessPiece piece3 = ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN);
    ChessPiece piece4 = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT);
    EXPECT_FALSE(piece1 != piece2);
    EXPECT_TRUE(piece1 != piece3);
    EXPECT_TRUE(piece1 != piece4);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::PAWN.
TEST(ChessPieceTest, GetRegularMovesPawn) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    Position special = Position(4, 1);
    ChessPiece pawn = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN);

    std::vector<Position> middleCheck = {Position(4, 5), Position(3, 5), Position(5, 5)};
    std::vector<Position> sideCheck = {Position(0, 5), Position(1, 5)};
    std::vector<Position> topCheck = {};
    std::vector<Position> bottomCheck = {Position(4, 1), Position(3, 1), Position(5, 1)};
    std::vector<Position> specialCheck = {Position(4, 2), Position(3, 2), Position(5, 2),
                                          Position(4, 3)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, pawn);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, pawn);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, pawn);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, pawn);
    std::vector<Position> specialResult = ChessPiece::get_regular_moves(special, pawn);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    std::sort(specialCheck.begin(), specialCheck.end());
    std::sort(specialResult.begin(), specialResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
    EXPECT_EQ(specialCheck, specialResult);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::ROOK.
TEST(ChessPieceTest, GetRegularMovesRook) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    ChessPiece rook = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK);

    std::vector<Position> middleCheck = {Position(4, 5), Position(4, 6), Position(4, 7),
                                         Position(4, 3), Position(4, 2), Position(4, 1),
                                         Position(4, 0), Position(3, 4), Position(2, 4),
                                         Position(1, 4), Position(0, 4), Position(5, 4),
                                         Position(6, 4), Position(7, 4)};
    std::vector<Position> sideCheck = {Position(0, 5), Position(0, 6), Position(0, 7),
                                       Position(0, 3), Position(0, 2), Position(0, 1),
                                       Position(0, 0), Position(3, 4), Position(2, 4),
                                       Position(1, 4), Position(4, 4), Position(5, 4),
                                       Position(6, 4), Position(7, 4)};
    std::vector<Position> topCheck = {Position(4, 5), Position(4, 6), Position(4, 4),
                                      Position(4, 3), Position(4, 2), Position(4, 1),
                                      Position(4, 0), Position(3, 7), Position(2, 7),
                                      Position(1, 7), Position(0, 7), Position(5, 7),
                                      Position(6, 7), Position(7, 7)};
    std::vector<Position> bottomCheck = {Position(4, 5), Position(4, 6), Position(4, 7),
                                         Position(4, 3), Position(4, 2), Position(4, 1),
                                         Position(4, 4), Position(3, 0), Position(2, 0),
                                         Position(1, 0), Position(0, 0), Position(5, 0),
                                         Position(6, 0), Position(7, 0)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, rook);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, rook);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, rook);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, rook);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::KING.
TEST(ChessPieceTest, GetRegularMovesKing) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    ChessPiece king = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING);

    std::vector<Position> middleCheck = {Position(4, 5), Position(5, 5), Position(3, 5),
                                         Position(3, 4), Position(5, 4), Position(3, 3),
                                         Position(4, 3), Position(5, 3)};
    std::vector<Position> sideCheck = {Position(0, 5), Position(0, 3), Position(1, 3),
                                       Position(1, 4), Position(1, 5)};
    std::vector<Position> topCheck = {Position(4, 6), Position(5, 6), Position(3, 6),
                                      Position(3, 7), Position(5, 7)};
    std::vector<Position> bottomCheck = {Position(3, 0), Position(5, 0), Position(3, 1),
                                         Position(4, 1), Position(5, 1)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, king);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, king);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, king);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, king);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::QUEEN.
TEST(ChessPieceTest, GetRegularMovesQueen) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    ChessPiece queen = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN);

    std::vector<Position> middleCheck = {Position(5, 5), Position(6, 6), Position(7, 7),
                                         Position(3, 3), Position(2, 2), Position(1, 1),
                                         Position(0, 0), Position(1, 7), Position(2, 6),
                                         Position(3, 5), Position(5, 3), Position(6, 2),
                                         Position(7, 1), Position(4, 5), Position(4, 6),
                                         Position(4, 7), Position(4, 3), Position(4, 2),
                                         Position(4, 1), Position(4, 0), Position(3, 4),
                                         Position(2, 4), Position(1, 4), Position(0, 4),
                                         Position(5, 4), Position(6, 4), Position(7, 4)};
    std::vector<Position> sideCheck = {Position(1, 5), Position(2, 6), Position(3, 7),
                                       Position(1, 3), Position(2, 2), Position(3, 1),
                                       Position(4, 0), Position(0, 5), Position(0, 6),
                                       Position(0, 7), Position(0, 3), Position(0, 2),
                                       Position(0, 1), Position(0, 0), Position(3, 4),
                                       Position(2, 4), Position(1, 4), Position(4, 4),
                                       Position(5, 4), Position(6, 4), Position(7, 4)};
    std::vector<Position> topCheck = {Position(5, 6), Position(6, 5), Position(7, 4),
                                      Position(3, 6), Position(2, 5), Position(1, 4),
                                      Position(0, 3), Position(4, 5), Position(4, 6),
                                      Position(4, 4), Position(4, 3), Position(4, 2),
                                      Position(4, 1), Position(4, 0), Position(3, 7),
                                      Position(2, 7), Position(1, 7), Position(0, 7),
                                      Position(5, 7), Position(6, 7), Position(7, 7)};
    std::vector<Position> bottomCheck = {Position(3, 1), Position(2, 2), Position(1, 3),
                                         Position(0, 4), Position(5, 1), Position(6, 2),
                                         Position(7, 3), Position(4, 5), Position(4, 6),
                                         Position(4, 7), Position(4, 3), Position(4, 2),
                                         Position(4, 1), Position(4, 4), Position(3, 0),
                                         Position(2, 0), Position(1, 0), Position(0, 0),
                                         Position(5, 0), Position(6, 0), Position(7, 0)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, queen);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, queen);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, queen);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, queen);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::KNIGHT.
TEST(ChessPieceTest, GetRegularMovesKnight) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    ChessPiece knight = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT);

    std::vector<Position> middleCheck = {Position(5, 6), Position(5, 2), Position(6, 5),
                                         Position(6, 3), Position(3, 6), Position(3, 2),
                                         Position(2, 5), Position(2, 3)};
    std::vector<Position> sideCheck = {Position(1, 6), Position(1, 2), Position(2, 3),
                                       Position(2, 5)};
    std::vector<Position> topCheck = {Position(2, 6), Position(6, 6), Position(3, 5),
                                      Position(5, 5)};
    std::vector<Position> bottomCheck = {Position(2, 1), Position(3, 2), Position(5, 2),
                                         Position(6, 1)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, knight);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, knight);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, knight);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, knight);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
}

// Test for getRegularMoves(Position, ChessPiece) where the piece is of MoveType::BISHOP.
TEST(ChessPieceTest, GetRegularMovesBishop) {
    Position middle = Position(4, 4);
    Position side = Position(0, 4);
    Position top = Position(4, 7);
    Position bottom = Position(4, 0);
    ChessPiece bishop = ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP);

    std::vector<Position> middleCheck = {Position(5, 5), Position(6, 6), Position(7, 7),
                                         Position(3, 3), Position(2, 2), Position(1, 1),
                                         Position(0, 0), Position(1, 7), Position(2, 6),
                                         Position(3, 5), Position(5, 3), Position(6, 2),
                                         Position(7, 1)};
    std::vector<Position> sideCheck = {Position(1, 5), Position(2, 6), Position(3, 7),
                                       Position(1, 3), Position(2, 2), Position(3, 1),
                                       Position(4, 0)};
    std::vector<Position> topCheck = {Position(5, 6), Position(6, 5), Position(7, 4),
                                      Position(3, 6), Position(2, 5), Position(1, 4),
                                      Position(0, 3)};
    std::vector<Position> bottomCheck = {Position(3, 1), Position(2, 2), Position(1, 3),
                                         Position(0, 4), Position(5, 1), Position(6, 2),
                                         Position(7, 3)};

    std::vector<Position> middleResult = ChessPiece::get_regular_moves(middle, bishop);
    std::vector<Position> sideResult = ChessPiece::get_regular_moves(side, bishop);
    std::vector<Position> topResult = ChessPiece::get_regular_moves(top, bishop);
    std::vector<Position> bottomResult = ChessPiece::get_regular_moves(bottom, bishop);

    std::sort(middleCheck.begin(), middleCheck.end());
    std::sort(middleResult.begin(), middleResult.end());

    std::sort(sideCheck.begin(), sideCheck.end());
    std::sort(sideResult.begin(), sideResult.end());

    std::sort(topCheck.begin(), topCheck.end());
    std::sort(topResult.begin(), topResult.end());

    std::sort(bottomCheck.begin(), bottomCheck.end());
    std::sort(bottomResult.begin(), bottomResult.end());

    EXPECT_EQ(middleCheck, middleResult);
    EXPECT_EQ(sideCheck, sideResult);
    EXPECT_EQ(topCheck, topResult);
    EXPECT_EQ(bottomCheck, bottomResult);
}
