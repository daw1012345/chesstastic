//
// Created by Dawid Kulikowski on 20/10/2021.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <chesstastic_chess/ChessMoveValidationService.h>

// Test for isValidMove(ChessMove move), check if some trivial moves are validated correctly.
TEST(ChessMoveValidationServiceTest, IsValidMoveTrivial) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 2), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 3), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 4), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 5), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::QUEEN)));

    ChessBoard board = ChessBoard(startPosition);

    ChessMove validMovePawn = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                        Position(0,0), Position(0, 1), MoveType::REGULAR);
    ChessMove invalidMovePawn = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                        Position(0,0), Position(1, 0), MoveType::REGULAR);
    ChessMove invalidMovePawn2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                          Position(0,0), Position(0, 0), MoveType::REGULAR);

    ChessMove validMoveRook = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                        Position(1,1), Position(1, 7), MoveType::REGULAR);
    ChessMove validMoveRook2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                        Position(1,1), Position(7, 1), MoveType::REGULAR);
    ChessMove invalidMoveRook = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                        Position(1,1), Position(2, 0), MoveType::REGULAR);
    ChessMove invalidMoveRook2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                          Position(1,1), Position(1, 1), MoveType::REGULAR);

    ChessMove validMoveKnight = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                        Position(2,2), Position(4, 3), MoveType::REGULAR);
    ChessMove validMoveKnight2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                         Position(2,2), Position(3, 4), MoveType::REGULAR);
    ChessMove invalidMoveKnight = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                          Position(2,2), Position(3, 5), MoveType::REGULAR);
    ChessMove invalidMoveKnight2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                            Position(2,2), Position(2, 2), MoveType::REGULAR);

    ChessMove validMoveBishop = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                        Position(3,3), Position(6, 0), MoveType::REGULAR);
    ChessMove validMoveBishop2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                         Position(3,3), Position(0, 6), MoveType::REGULAR);
    ChessMove invalidMoveBishop = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                          Position(3,3), Position(2, 5), MoveType::REGULAR);
    ChessMove invalidMoveBishop2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                            Position(3,3), Position(3, 3), MoveType::REGULAR);

    ChessMove validMoveKing = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                        Position(4,4), Position(3, 5), MoveType::REGULAR);
    ChessMove validMoveKing2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                         Position(4,4), Position(4, 5), MoveType::REGULAR);
    ChessMove invalidMoveKing = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                          Position(4,4), Position(4, 6), MoveType::REGULAR);
    ChessMove invalidMoveKing2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                          Position(4,4), Position(4, 4), MoveType::REGULAR);

    ChessMove validMoveQueen = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                        Position(5,5), Position(7, 7), MoveType::REGULAR);
    ChessMove validMoveQueen2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                         Position(5,5), Position(5, 0), MoveType::REGULAR);
    ChessMove invalidMoveQueen = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                          Position(5,5), Position(6, 7), MoveType::REGULAR);
    ChessMove invalidMoveQueen2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                           Position(5,5), Position(5, 5), MoveType::REGULAR);

    ChessMoveValidationService validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(validMovePawn, board));
    EXPECT_FALSE(validator.isValidMove(invalidMovePawn, board));
    EXPECT_FALSE(validator.isValidMove(invalidMovePawn2, board));

    EXPECT_TRUE(validator.isValidMove(validMoveRook, board));
    EXPECT_TRUE(validator.isValidMove(validMoveRook2, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveRook, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveRook2, board));

    EXPECT_TRUE(validator.isValidMove(validMoveKnight, board));
    EXPECT_TRUE(validator.isValidMove(validMoveKnight2, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveKnight, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveKnight2, board));

    EXPECT_TRUE(validator.isValidMove(validMoveBishop, board));
    EXPECT_TRUE(validator.isValidMove(validMoveBishop2, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveBishop, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveBishop2, board));

    EXPECT_TRUE(validator.isValidMove(validMoveKing, board));
    EXPECT_TRUE(validator.isValidMove(validMoveKing2, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveKing, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveKing2, board));

    EXPECT_TRUE(validator.isValidMove(validMoveQueen, board));
    EXPECT_TRUE(validator.isValidMove(validMoveQueen2, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveQueen, board));
    EXPECT_FALSE(validator.isValidMove(invalidMoveQueen2, board));
}

// Test for isValidMove(ChessMove move), check if moves are marked invalid when destination holds one of players own pieces
TEST(ChessMoveValidationServiceTest, IsValidMoveDestinationSelf) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 2), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 3), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 4), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 5), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::QUEEN)));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));

    ChessBoard board = ChessBoard(startPosition);

    ChessMove blockedPawn = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                      Position(0, 0), Position(0, 1), MoveType::REGULAR);
    ChessMove blockedRook = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                      Position(1, 1), Position(0, 1), MoveType::REGULAR);
    ChessMove blockedKnight = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
                                        Position(2, 2), Position(0, 1), MoveType::REGULAR);
    ChessMove blockedBishop = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                        Position(3, 3), Position(4, 4), MoveType::REGULAR);
    ChessMove blockedKing = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                      Position(4, 4), Position(5, 5), MoveType::REGULAR);
    ChessMove blockedQueen = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                       Position(5, 5), Position(4, 4), MoveType::REGULAR);

    ChessMoveValidationService validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(blockedPawn, board));
    EXPECT_FALSE(validator.isValidMove(blockedRook, board));
    EXPECT_FALSE(validator.isValidMove(blockedKnight, board));
    EXPECT_FALSE(validator.isValidMove(blockedBishop, board));
    EXPECT_FALSE(validator.isValidMove(blockedKing, board));
    EXPECT_FALSE(validator.isValidMove(blockedQueen, board));
}

// Test for isValidMove(ChessMove move), check if moves are marked invalid when blocked by one of players own pieces
TEST(ChessMoveValidationServiceTest, IsValidMoveBlockedSelf) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 3), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 5), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::QUEEN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));

    ChessMove blockedRook = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                      Position(1, 1), Position(7, 1), MoveType::REGULAR);
    ChessMove blockedBishop = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                        Position(3, 3), Position(6, 0), MoveType::REGULAR);
    ChessMove blockedQueen = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                       Position(5, 5), Position(5, 0), MoveType::REGULAR);

    ChessBoard board = ChessBoard(startPosition);

    ChessMoveValidationService validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(blockedRook, board));
    EXPECT_TRUE(validator.isValidMove(blockedBishop, board));
    EXPECT_TRUE(validator.isValidMove(blockedQueen, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(blockedRook, board));
    EXPECT_FALSE(validator.isValidMove(blockedBishop, board));
    EXPECT_FALSE(validator.isValidMove(blockedQueen, board));
}

// Test for isValidMove(ChessMove move), check if moves are marked invalid when blocked by one of opponents pieces
TEST(ChessMoveValidationServiceTest, IsValidMoveBlockedOther) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 3), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 5), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::QUEEN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));

    ChessMove blockedRook = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
                                      Position(1, 1), Position(7, 1), MoveType::REGULAR);
    ChessMove blockedBishop = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
                                        Position(3, 3), Position(6, 0), MoveType::REGULAR);
    ChessMove blockedQueen = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
                                       Position(5, 5), Position(5, 0), MoveType::REGULAR);

    ChessBoard board = ChessBoard(startPosition);

    ChessMoveValidationService validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(blockedRook, board));
    EXPECT_TRUE(validator.isValidMove(blockedBishop, board));
    EXPECT_TRUE(validator.isValidMove(blockedQueen, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 1), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(blockedRook, board));
    EXPECT_FALSE(validator.isValidMove(blockedBishop, board));
    EXPECT_FALSE(validator.isValidMove(blockedQueen, board));
}

// Test for isValidMove(ChessMove), check if pawn attack moves are validated correctly.
TEST(ChessMoveValidationServiceTest, IsValidPawnAttackMove) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));

    ChessMove moveRight = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                      Position(4, 1), Position(5, 2), MoveType::REGULAR);
    ChessMove moveLeft = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                        Position(4, 1), Position(3, 2), MoveType::REGULAR);
    ChessMove moveUp = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                 Position(4, 1), Position(4, 2), MoveType::REGULAR);

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(moveRight, board));
    EXPECT_FALSE(validator.isValidMove(moveLeft, board));
    EXPECT_TRUE(validator.isValidMove(moveUp, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 2), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 2), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 2), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(moveRight, board));
    EXPECT_TRUE(validator.isValidMove(moveLeft, board));
    EXPECT_FALSE(validator.isValidMove(moveUp, board));
}

// Test for isValidMove(ChessMove), check if enPassant moves are validated correctly.
TEST(ChessMoveValidationServiceTest, IsValidEnPassantMove) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 4), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    ChessMove moveRight = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                          Position(4, 4), Position(5, 5), MoveType::REGULAR);
    ChessMove moveLeft = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                         Position(4, 4), Position(3, 5), MoveType::REGULAR);
    ChessMove moveUp = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                       Position(4, 4), Position(4, 5), MoveType::REGULAR);

    EXPECT_FALSE(validator.isValidMove(moveRight, board));
    EXPECT_FALSE(validator.isValidMove(moveLeft, board));
    EXPECT_TRUE(validator.isValidMove(moveUp, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 4), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 4), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(moveRight, board));
    EXPECT_TRUE(validator.isValidMove(moveLeft, board));
    EXPECT_TRUE(validator.isValidMove(moveUp, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 5), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 5), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(moveRight, board));
    EXPECT_TRUE(validator.isValidMove(moveLeft, board));
    EXPECT_FALSE(validator.isValidMove(moveUp, board));
}

// Test for isValidMove(ChessMove), check if double pawn moves are validated correctly.
TEST(ChessMoveValidationServiceTest, IsValidDoublePawnMove) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 4), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 4), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 1), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    ChessMove validDoubleWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                    Position(1, 1), Position(1, 3), MoveType::REGULAR);
    ChessMove invalidDoubleWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                   Position(4, 4), Position(4, 6), MoveType::REGULAR);
    ChessMove invalidDoubleWhite2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                 Position(6, 6), Position(6, 4), MoveType::REGULAR);
    ChessMove invalidDoubleBlack2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                           Position(2, 1), Position(2, 3), MoveType::REGULAR);
    ChessMove invalidDoubleBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                             Position(3, 4), Position(3, 2), MoveType::REGULAR);
    ChessMove validDoubleBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                              Position(5, 6), Position(5, 4), MoveType::REGULAR);

    EXPECT_FALSE(validator.isValidMove(invalidDoubleWhite, board));
    EXPECT_FALSE(validator.isValidMove(invalidDoubleWhite2, board));
    EXPECT_TRUE(validator.isValidMove(validDoubleWhite, board));
    EXPECT_FALSE(validator.isValidMove(invalidDoubleBlack, board));
    EXPECT_FALSE(validator.isValidMove(invalidDoubleBlack2, board));
    EXPECT_TRUE(validator.isValidMove(validDoubleBlack, board));

    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 2), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 5), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(validDoubleWhite, board));
    EXPECT_FALSE(validator.isValidMove(validDoubleBlack, board));
}

// Test for isValidMove(ChessMove), checks if castling is validated correctly when king and/or rooks are incorrectly placed.
TEST(ChessMoveValidationServiceTest, IsInvalidMoveCastling) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    ChessMove castleShortWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(0),
                                           Position(0), MoveType::CASTLE_SHORT);
    ChessMove castleLongWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(0),
                                          Position(0), MoveType::CASTLE_LONG);
    ChessMove castleShortBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(0),
                                           Position(0), MoveType::CASTLE_SHORT);
    ChessMove castleLongBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(0),
                                          Position(0), MoveType::CASTLE_LONG);

    EXPECT_FALSE(validator.isValidMove(castleShortWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleLongWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleShortBlack, board));
    EXPECT_FALSE(validator.isValidMove(castleLongBlack, board));

    startPosition.clear();
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 0), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(castleShortWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleLongWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleShortBlack, board));
    EXPECT_FALSE(validator.isValidMove(castleLongBlack, board));

    startPosition.clear();
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));

    board = ChessBoard(startPosition);
    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(castleShortWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleLongWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleShortBlack, board));
    EXPECT_FALSE(validator.isValidMove(castleLongBlack, board));
}

// Test for isValidMove(ChessMove move), checks if castling is validated correctly when king and rooks are correctly placed.
TEST(ChessMoveValidationServiceTest, IsValidMoveCastling) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    ChessMove castleShortWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(0),
                                           Position(0), MoveType::CASTLE_SHORT);
    ChessMove castleLongWhite = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(0),
                                          Position(0), MoveType::CASTLE_LONG);
    ChessMove castleShortBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(0),
                                           Position(0), MoveType::CASTLE_SHORT);
    ChessMove castleLongBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(0),
                                          Position(0), MoveType::CASTLE_LONG);

    EXPECT_TRUE(validator.isValidMove(castleShortWhite, board));
    EXPECT_TRUE(validator.isValidMove(castleLongWhite, board));
    EXPECT_TRUE(validator.isValidMove(castleShortBlack, board));
    EXPECT_TRUE(validator.isValidMove(castleLongBlack, board));

    board.set_field(Position(1, 0), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));
    board.set_field(Position(6, 0), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::BISHOP));

    validator = ChessMoveValidationService();

    EXPECT_FALSE(validator.isValidMove(castleShortWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleLongWhite, board));
    EXPECT_TRUE(validator.isValidMove(castleShortBlack, board));
    EXPECT_TRUE(validator.isValidMove(castleLongBlack, board));

    board.remove_at(Position(1, 0));
    board.remove_at(Position(6, 0));
    board.set_field(Position(0, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(1, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(2, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(3, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(4, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(5, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(6, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(7, 1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(1, 2), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT));
    board.set_field(Position(6, 2), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::QUEEN));
    board.set_field(Position(0, 6), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    board.set_field(Position(7, 6), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(castleShortWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleLongWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleShortBlack, board));
    EXPECT_TRUE(validator.isValidMove(castleLongBlack, board));

    board = ChessBoard(startPosition);

    board.set_field(Position(4, 5), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN));
    board.set_field(Position(4, 6), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    board.set_field(Position(0, 6), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    validator = ChessMoveValidationService();

    EXPECT_TRUE(validator.isValidMove(castleShortWhite, board));
    EXPECT_TRUE(validator.isValidMove(castleLongWhite, board));
    EXPECT_FALSE(validator.isValidMove(castleShortBlack, board));
    EXPECT_FALSE(validator.isValidMove(castleLongBlack, board));
}

// Test for isValidMove(ChessMove), Checks if promotion moves are validated correctly.
TEST(ChessMoveValidationServiceTest, IsValidMovePromotions) {
    std::map<Position, ChessPiece> startPosition;

    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 5), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 7), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    ChessMove rookPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(4, 6),
                                           Position(4, 7), MoveType::PROMOTION_ROOK);
    ChessMove knightPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(5, 6),
                                          Position(5, 7), MoveType::PROMOTION_KNIGHT);
    ChessMove bishopPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(6, 6),
                                           Position(6, 7), MoveType::PROMOTION_BISHOP);
    ChessMove queenPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(7, 6),
                                          Position(7, 7), MoveType::PROMOTION_QUEEN);
    ChessMove promotionBlack = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(1, 1),
                                          Position(1, 0), MoveType::PROMOTION_QUEEN);
    ChessMove invalidPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(0, 5),
                                         Position(0, 7), MoveType::PROMOTION_QUEEN);
    ChessMove attackPromotion = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(1, 6),
                                          Position(0, 7), MoveType::PROMOTION_QUEEN);

    EXPECT_TRUE(validator.isValidMove(rookPromotion, board));
    EXPECT_TRUE(validator.isValidMove(knightPromotion, board));
    EXPECT_TRUE(validator.isValidMove(bishopPromotion, board));
    EXPECT_TRUE(validator.isValidMove(queenPromotion, board));
    EXPECT_TRUE(validator.isValidMove(promotionBlack, board));
    EXPECT_FALSE(validator.isValidMove(invalidPromotion, board));
    EXPECT_TRUE(validator.isValidMove(attackPromotion, board));
}

// Test for getPossibleMoves(ChessPiece, Position), checks whether the correct moves are returned for the starting position of each piece.
TEST(ChessMoveValidationServiceTest, GetPossibleMovesStartPosition) {
    std::map<Position, ChessPiece> startPosition;

    // Placing pawns on the board.
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 1), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 6), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    // Arranging all other pieces on the board.
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::QUEEN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::QUEEN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));

    ChessBoard board = ChessBoard(startPosition);
    ChessMoveValidationService validator = ChessMoveValidationService();

    for (std::pair<Position, ChessPiece> pair: board.get_board()) {
        std::vector<ChessMove> possibleMoves;
        ChessPieceSide side = pair.second.get_side();

        for (int i = 0; i < 64; i++) {
            ChessMove move = ChessMove(pair.second, pair.first, Position(i), MoveType::REGULAR);
            if (pair.second.get_type() == ChessPieceType::PAWN &&
                i < (side == ChessPieceSide::WHITE ? 64 : 8) && i > (side == ChessPieceSide::WHITE ? 55 : -1)) {
                move = ChessMove(pair.second, pair.first, Position(i), MoveType::PROMOTION_QUEEN);
                if (validator.isValidMove(move, board)) {
                    possibleMoves.push_back(move);
                    move = ChessMove(pair.second, pair.first, Position(i), MoveType::PROMOTION_BISHOP);
                    possibleMoves.push_back(move);
                    move = ChessMove(pair.second, pair.first, Position(i), MoveType::PROMOTION_KNIGHT);
                    possibleMoves.push_back(move);
                    move = ChessMove(pair.second, pair.first, Position(i), MoveType::PROMOTION_ROOK);
                    possibleMoves.push_back(move);
                }
            } else if (validator.isValidMove(move, board)) possibleMoves.push_back(move);
        }
        if (pair.second.get_type() == ChessPieceType::KING) {
            ChessMove castleShort = ChessMove(ChessPiece(side, ChessPieceType::KING),
                                              Position(0), Position(0), MoveType::CASTLE_SHORT);
            ChessMove castleLong = ChessMove(ChessPiece(side, ChessPieceType::KING),
                                             Position(0), Position(0), MoveType::CASTLE_LONG);
            if (validator.isValidMove(castleShort, board)) possibleMoves.push_back(castleShort);
            if (validator.isValidMove(castleLong, board)) possibleMoves.push_back(castleLong);
        }

        std::vector<ChessMove> result = validator.getPossibleMoves(pair.second, pair.first, board);

        std::sort(result.begin(), result.end());
        std::sort(possibleMoves.begin(), possibleMoves.end());

        EXPECT_EQ(result, possibleMoves);
    }
}

// Test for getPossibleMove(ChessPiece, Position), checks if the correct are returned for each piece where boardstatus is so that most special moves are possible.
TEST(ChessMoveValidationServiceTest, GetPossibleMovesMidGamePosition) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());

    board.set_field(Position(0, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    board.set_field(Position(4, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING));
    board.set_field(Position(7, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));

    board.set_field(Position(1, 1),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(6, 1),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    board.set_field(Position(0, 2),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN));
    board.set_field(Position(5, 2),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));

    board.set_field(Position(2, 3),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    board.set_field(Position(3, 3),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    board.set_field(Position(7, 4),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));

    board.set_field(Position(6, 5),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT));

    board.set_field(Position(0, 6),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(6, 6),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(7, 6),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));

    board.set_field(Position(0, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    board.set_field(Position(4, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING));
    board.set_field(Position(7, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));

    ChessMoveValidationService validator = ChessMoveValidationService();

    // Blocked pawn.
    std::vector<ChessMove> whitePawn1 = {};
    // Pawn with double move ability.
    std::vector<ChessMove> whitePawn2 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(1, 1), Position(1, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(1, 1), Position(1, 3), MoveType::REGULAR)};
    // Pawn in en passant row of black.
    std::vector<ChessMove> whitePawn3 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(3, 3), Position(3, 4), MoveType::REGULAR)};
    // Pawn with attack move.
    std::vector<ChessMove> whitePawn4 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 1), Position(6, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 1), Position(5, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 1), Position(6, 3), MoveType::REGULAR)};
    // Pawn with promotion/attack promotion.
    std::vector<ChessMove> whitePawn5 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(6, 7), MoveType::PROMOTION_QUEEN), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(6, 7), MoveType::PROMOTION_ROOK), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(6, 7), MoveType::PROMOTION_KNIGHT), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(6, 7), MoveType::PROMOTION_BISHOP), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(7, 7), MoveType::PROMOTION_QUEEN), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(7, 7), MoveType::PROMOTION_ROOK), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(7, 7), MoveType::PROMOTION_BISHOP), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 6), Position(7, 7), MoveType::PROMOTION_KNIGHT)};
    // Rook in castling position.
    std::vector<ChessMove> whiteRook1 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(0, 0), Position(0, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(0, 0), Position(1, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(0, 0), Position(2, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(0, 0), Position(3, 0), MoveType::REGULAR)};
    // Rook in castling position.
    std::vector<ChessMove> whiteRook2 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(7, 0), Position(5, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(7, 0), Position(6, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(7, 0), Position(7, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(7, 0), Position(7, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK),
            Position(7, 0), Position(7, 3), MoveType::REGULAR)};
    // Queen in position to move a lot, threaten kingside castle and attack a pawn.
    std::vector<ChessMove> whiteQueen = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(0, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(0, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(0, 4), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(0, 5), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(1, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(2, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(3, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(4, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(5, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(1, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(2, 4), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(3, 5), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(4, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(5, 7), MoveType::REGULAR)};
    // King in position to castle on both sides.
    std::vector<ChessMove> whiteKing = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(3, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(3, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(5, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(5, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(0), Position(0), MoveType::CASTLE_LONG), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(0), Position(0), MoveType::CASTLE_SHORT)};
    // Knight in position to take black rook and threaten kingside castle.
    std::vector<ChessMove> whiteKnight = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(4, 4), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(4, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(5, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(7, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(7, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT),
            Position(6, 5), Position(5, 3), MoveType::REGULAR)};
    // Bishop in position to take black pawn.
    std::vector<ChessMove> whiteBishop = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
            Position(7, 4), Position(6, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
            Position(7, 4), Position(5, 2), MoveType::REGULAR)};

    // Pawn in position to move forward and make en passant move.
    std::vector<ChessMove> blackPawn1 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(2, 3), Position(2, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(2, 3), Position(3, 2), MoveType::REGULAR)};
    // Pawn in position to attack.
    std::vector<ChessMove> blackPawn2 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(5, 2), Position(5, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(5, 2), Position(6, 1), MoveType::REGULAR)};
    // Pawn in position to attack white knight.
    std::vector<ChessMove> blackPawn3 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(7, 6), Position(6, 5), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(7, 6), Position(7, 5), MoveType::REGULAR)};
    // Rook in position to take white pawn and castle.
    std::vector<ChessMove> blackRook1 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(1, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(2, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(3, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(0, 6), MoveType::REGULAR)};
    // Rook in position to castle but unable because of threats.
    std::vector<ChessMove> blackRook2 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(7, 7), Position(6, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(7, 7), Position(5, 7), MoveType::REGULAR)};
    // King in position to castle queenside.
    std::vector<ChessMove> blackKing = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(5, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(3, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(3, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(0), Position(0), MoveType::CASTLE_LONG)};

    std::vector<ChessMove> whitePawn1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(0, 6), board);
    std::vector<ChessMove> whitePawn2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(1, 1), board);
    std::vector<ChessMove> whitePawn3Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(3, 3), board);
    std::vector<ChessMove> whitePawn4Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(6, 1), board);
    std::vector<ChessMove> whitePawn5Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(6, 6), board);
    std::vector<ChessMove> whiteRook1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK), Position(0, 0), board);
    std::vector<ChessMove> whiteRook2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK), Position(7, 0), board);
    std::vector<ChessMove> whiteQueenResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN), Position(0, 2), board);
    std::vector<ChessMove> whiteKingResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(4, 0), board);
    std::vector<ChessMove> whiteKnightResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT), Position(6, 5), board);
    std::vector<ChessMove> whiteBishopResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP), Position(7, 4), board);

    std::vector<ChessMove> blackPawn1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(2, 3), board);
    std::vector<ChessMove> blackPawn2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(5, 2), board);
    std::vector<ChessMove> blackPawn3Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(7, 6), board);
    std::vector<ChessMove> blackRook1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK), Position(0, 7), board);
    std::vector<ChessMove> blackRook2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK), Position(7, 7), board);
    std::vector<ChessMove> blackKingResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(4, 7), board);

    std::sort(whitePawn1.begin(), whitePawn1.end());
    std::sort(whitePawn1Result.begin(), whitePawn1Result.end());

    std::sort(whitePawn2.begin(), whitePawn2.end());
    std::sort(whitePawn2Result.begin(), whitePawn2Result.end());

    std::sort(whitePawn3.begin(), whitePawn3.end());
    std::sort(whitePawn3Result.begin(), whitePawn3Result.end());

    std::sort(whitePawn4.begin(), whitePawn4.end());
    std::sort(whitePawn4Result.begin(), whitePawn4Result.end());

    std::sort(whitePawn5.begin(), whitePawn5.end());
    std::sort(whitePawn5Result.begin(), whitePawn5Result.end());

    std::sort(whiteRook1.begin(), whiteRook1.end());
    std::sort(whiteRook1Result.begin(), whiteRook1Result.end());

    std::sort(whiteRook2.begin(), whiteRook2.end());
    std::sort(whiteRook2Result.begin(), whiteRook2Result.end());

    std::sort(whiteQueen.begin(), whiteQueen.end());
    std::sort(whiteQueenResult.begin(), whiteQueenResult.end());

    std::sort(whiteKing.begin(), whiteKing.end());
    std::sort(whiteKingResult.begin(), whiteKingResult.end());

    std::sort(whiteKnight.begin(), whiteKnight.end());
    std::sort(whiteKnightResult.begin(), whiteKnightResult.end());

    std::sort(whiteBishop.begin(), whiteBishop.end());
    std::sort(whiteBishopResult.begin(), whiteBishopResult.end());


    std::sort(blackPawn1.begin(), blackPawn1.end());
    std::sort(blackPawn1Result.begin(), blackPawn1Result.end());

    std::sort(blackPawn2.begin(), blackPawn2.end());
    std::sort(blackPawn2Result.begin(), blackPawn2Result.end());

    std::sort(blackPawn3.begin(), blackPawn3.end());
    std::sort(blackPawn3Result.begin(), blackPawn3Result.end());

    std::sort(blackRook1.begin(), blackRook1.end());
    std::sort(blackRook1Result.begin(), blackRook1Result.end());

    std::sort(blackRook2.begin(), blackRook2.end());
    std::sort(blackRook2Result.begin(), blackRook2Result.end());

    std::sort(blackKing.begin(), blackKing.end());
    std::sort(blackKingResult.begin(), blackKingResult.end());

    EXPECT_EQ(whitePawn1, whitePawn1Result);
    EXPECT_EQ(whitePawn2, whitePawn2Result);
    EXPECT_EQ(whitePawn3, whitePawn3Result);
    EXPECT_EQ(whitePawn4, whitePawn4Result);
    EXPECT_EQ(whitePawn5, whitePawn5Result);
    EXPECT_EQ(whiteRook1, whiteRook1Result);
    EXPECT_EQ(whiteRook2, whiteRook2Result);
    EXPECT_EQ(whiteQueen, whiteQueenResult);
    EXPECT_EQ(whiteKing, whiteKingResult);
    EXPECT_EQ(whiteKnight, whiteKnightResult);
    EXPECT_EQ(whiteBishop, whiteBishopResult);

    EXPECT_EQ(blackPawn1, blackPawn1Result);
    EXPECT_EQ(blackPawn2, blackPawn2Result);
    EXPECT_EQ(blackPawn3, blackPawn3Result);
    EXPECT_EQ(blackRook1, blackRook1Result);
    EXPECT_EQ(blackRook2, blackRook2Result);
    EXPECT_EQ(blackKing, blackKingResult);
}

// Test for getPossibleMoves(ChessPiece, Position), checks if the correct moves are returned for all pieces when king is
// in check, this means that no move can be done after which the king is still in check.
TEST(ChessMoveValidationServiceTest, GetPossibleMovesKingInCheck) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());

    board.set_field(Position(0, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    board.set_field(Position(4, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING));
    board.set_field(Position(7, 0),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));

    board.set_field(Position(1, 1),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(6, 1),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    board.set_field(Position(0, 2),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN));
    // Sets white king in check.
    board.set_field(Position(5, 2),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT));

    board.set_field(Position(2, 3),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    board.set_field(Position(3, 3),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    board.set_field(Position(7, 4),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));

    board.set_field(Position(6, 5),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT));

    board.set_field(Position(0, 6),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(6, 6),
                    ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(7, 6),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));

    board.set_field(Position(0, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    board.set_field(Position(4, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING));
    board.set_field(Position(7, 7),
                    ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));

    ChessMoveValidationService validator = ChessMoveValidationService();

    std::vector<ChessMove> whitePawn1 = {};
    std::vector<ChessMove> whitePawn2 = {};
    std::vector<ChessMove> whitePawn3 = {};
    // Pawn can attack knight who checks.
    std::vector<ChessMove> whitePawn4 = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
            Position(6, 1), Position(5, 2), MoveType::REGULAR)};
    std::vector<ChessMove> whitePawn5 = {};
    std::vector<ChessMove> whiteRook1 = {};
    std::vector<ChessMove> whiteRook2 = {};
    // Queen can attack knight who checks.
    std::vector<ChessMove> whiteQueen = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN),
            Position(0, 2), Position(5, 2), MoveType::REGULAR)};
    // King can escape check, but no longer castle due to check.
    std::vector<ChessMove> whiteKing = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(3, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(4, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(5, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
            Position(4, 0), Position(5, 0), MoveType::REGULAR)};
    std::vector<ChessMove> whiteKnight = {};
    // Bishop in position to take attacking knight.
    std::vector<ChessMove> whiteBishop = {ChessMove(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP),
            Position(7, 4), Position(5, 2), MoveType::REGULAR)};

    // Pawn in position to move forward and make en passant move.
    std::vector<ChessMove> blackPawn1 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(2, 3), Position(2, 2), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(2, 3), Position(3, 2), MoveType::REGULAR)};
    // Pawn in position to attack white knight.
    std::vector<ChessMove> blackPawn2 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(7, 6), Position(6, 5), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
            Position(7, 6), Position(7, 5), MoveType::REGULAR)};
    // Knight who checks white king.
    std::vector<ChessMove> blackKnight = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(4, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(6, 0), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(7, 1), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(7, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(6, 4), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(4, 4), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(3, 3), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT),
            Position(5, 2), Position(3, 1), MoveType::REGULAR)};
    // Rook in position to take white pawn and castle.
    std::vector<ChessMove> blackRook1 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(1, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(2, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(3, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(0, 7), Position(0, 6), MoveType::REGULAR)};
    // Rook in position to castle but unable because of threats.
    std::vector<ChessMove> blackRook2 = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(7, 7), Position(6, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK),
            Position(7, 7), Position(5, 7), MoveType::REGULAR)};
    // King in position to castle queenside.
    std::vector<ChessMove> blackKing = {ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(5, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(3, 6), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(4, 7), Position(3, 7), MoveType::REGULAR), ChessMove(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
            Position(0), Position(0), MoveType::CASTLE_LONG)};

    std::vector<ChessMove> whitePawn1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(0, 6), board);
    std::vector<ChessMove> whitePawn2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(1, 1), board);
    std::vector<ChessMove> whitePawn3Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(3, 3), board);
    std::vector<ChessMove> whitePawn4Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(6, 1), board);
    std::vector<ChessMove> whitePawn5Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), Position(6, 6), board);
    std::vector<ChessMove> whiteRook1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK), Position(0, 0), board);
    std::vector<ChessMove> whiteRook2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK), Position(7, 0), board);
    std::vector<ChessMove> whiteQueenResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN), Position(0, 2), board);
    std::vector<ChessMove> whiteKingResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING), Position(4, 0), board);
    std::vector<ChessMove> whiteKnightResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT), Position(6, 5), board);
    std::vector<ChessMove> whiteBishopResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP), Position(7, 4), board);

    std::vector<ChessMove> blackPawn1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(2, 3), board);
    std::vector<ChessMove> blackKnightResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT), Position(5, 2), board);
    std::vector<ChessMove> blackPawn2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), Position(7, 6), board);
    std::vector<ChessMove> blackRook1Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK), Position(0, 7), board);
    std::vector<ChessMove> blackRook2Result = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK), Position(7, 7), board);
    std::vector<ChessMove> blackKingResult = validator.getPossibleMoves(
            ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING), Position(4, 7), board);

    std::sort(whitePawn1.begin(), whitePawn1.end());
    std::sort(whitePawn1Result.begin(), whitePawn1Result.end());

    std::sort(whitePawn2.begin(), whitePawn2.end());
    std::sort(whitePawn2Result.begin(), whitePawn2Result.end());

    std::sort(whitePawn3.begin(), whitePawn3.end());
    std::sort(whitePawn3Result.begin(), whitePawn3Result.end());

    std::sort(whitePawn4.begin(), whitePawn4.end());
    std::sort(whitePawn4Result.begin(), whitePawn4Result.end());

    std::sort(whitePawn5.begin(), whitePawn5.end());
    std::sort(whitePawn5Result.begin(), whitePawn5Result.end());

    std::sort(whiteRook1.begin(), whiteRook1.end());
    std::sort(whiteRook1Result.begin(), whiteRook1Result.end());

    std::sort(whiteRook2.begin(), whiteRook2.end());
    std::sort(whiteRook2Result.begin(), whiteRook2Result.end());

    std::sort(whiteQueen.begin(), whiteQueen.end());
    std::sort(whiteQueenResult.begin(), whiteQueenResult.end());

    std::sort(whiteKing.begin(), whiteKing.end());
    std::sort(whiteKingResult.begin(), whiteKingResult.end());

    std::sort(whiteKnight.begin(), whiteKnight.end());
    std::sort(whiteKnightResult.begin(), whiteKnightResult.end());

    std::sort(whiteBishop.begin(), whiteBishop.end());
    std::sort(whiteBishopResult.begin(), whiteBishopResult.end());


    std::sort(blackPawn1.begin(), blackPawn1.end());
    std::sort(blackPawn1Result.begin(), blackPawn1Result.end());

    std::sort(blackPawn2.begin(), blackPawn2.end());
    std::sort(blackPawn2Result.begin(), blackPawn2Result.end());

    std::sort(blackKnight.begin(), blackKnight.end());
    std::sort(blackKnightResult.begin(), blackKnightResult.end());

    std::sort(blackRook1.begin(), blackRook1.end());
    std::sort(blackRook1Result.begin(), blackRook1Result.end());

    std::sort(blackRook2.begin(), blackRook2.end());
    std::sort(blackRook2Result.begin(), blackRook2Result.end());

    std::sort(blackKing.begin(), blackKing.end());
    std::sort(blackKingResult.begin(), blackKingResult.end());

    EXPECT_EQ(whitePawn1, whitePawn1Result);
    EXPECT_EQ(whitePawn2, whitePawn2Result);
    EXPECT_EQ(whitePawn3, whitePawn3Result);
    EXPECT_EQ(whitePawn4, whitePawn4Result);
    EXPECT_EQ(whitePawn5, whitePawn5Result);
    EXPECT_EQ(whiteRook1, whiteRook1Result);
    EXPECT_EQ(whiteRook2, whiteRook2Result);
    EXPECT_EQ(whiteQueen, whiteQueenResult);
    EXPECT_EQ(whiteKing, whiteKingResult);
    EXPECT_EQ(whiteKnight, whiteKnightResult);
    EXPECT_EQ(whiteBishop, whiteBishopResult);

    EXPECT_EQ(blackPawn1, blackPawn1Result);
    EXPECT_EQ(blackPawn2, blackPawn2Result);
    EXPECT_EQ(blackKnight, blackKnightResult);
    EXPECT_EQ(blackRook1, blackRook1Result);
    EXPECT_EQ(blackRook2, blackRook2Result);
    EXPECT_EQ(blackKing, blackKingResult);
}
