//
// Created by degra on 05/11/2021.
//

#include <gtest/gtest.h>
#include <chesstastic_chess/ChessMove.h>

// Test for getSource(), check if the correct source position is returned.
TEST(ChessMoveTest, GetSource) {
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                               Position(0), Position(10), MoveType::REGULAR);
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                               Position(8), Position(0), MoveType::PROMOTION_QUEEN);
    EXPECT_EQ(Position(0), move1.getSource());
    EXPECT_EQ(Position(8), move2.getSource());
}

// Test for getDestination(), check if the correct destination position is returned.
TEST(ChessMoveTest, GetDestination) {
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                Position(0), Position(10), MoveType::REGULAR);
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                Position(8), Position(0), MoveType::PROMOTION_QUEEN);
    EXPECT_EQ(Position(10), move1.getDestination());
    EXPECT_EQ(Position(0), move2.getDestination());
}

// Test for getType(), check if the correct MoveType is returned.
TEST(ChessMoveTest, GetType) {
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                Position(0), Position(10), MoveType::REGULAR);
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                Position(8), Position(0), MoveType::PROMOTION_QUEEN);
    EXPECT_EQ(MoveType::REGULAR, move1.getType());
    EXPECT_EQ(MoveType::PROMOTION_QUEEN, move2.getType());
}

// Test for getPiece(), check if the correct ChessPiece is returned.
TEST(ChessMoveTest, GetPiece) {
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                Position(0), Position(10), MoveType::REGULAR);
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                Position(8), Position(0), MoveType::PROMOTION_QUEEN);
    EXPECT_EQ(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT), move1.getPiece());
    EXPECT_EQ(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), move2.getPiece());
}

// Test for setSource(Position), check if the source position is correctly set.
TEST(ChessMoveTest, SetSource) {
    ChessMove move = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                Position(0), Position(10), MoveType::REGULAR);
    EXPECT_NE(Position(1), move.getSource());
    move.setSource(Position(1));
    EXPECT_EQ(Position(1), move.getSource());
}

// Test for setDestination(Position), check if the destination position is correctly set.
TEST(ChessMoveTest, SetDestination) {
    ChessMove move = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                               Position(0), Position(10), MoveType::REGULAR);
    EXPECT_NE(Position(1), move.getDestination());
    move.setDestination(Position(1));
    EXPECT_EQ(Position(1), move.getDestination());
}

// Test for setType(MoveType), check if the MoveType is correctly set.
TEST(ChessMoveTest, SetType) {
    ChessMove move = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                               Position(0), Position(10), MoveType::REGULAR);
    EXPECT_NE(MoveType::PROMOTION_QUEEN, move.getType());
    move.setType(MoveType::PROMOTION_QUEEN);
    EXPECT_EQ(MoveType::PROMOTION_QUEEN, move.getType());
}
