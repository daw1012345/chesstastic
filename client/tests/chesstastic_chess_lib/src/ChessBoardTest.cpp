//
// Created by degra on 03/11/2021.
//
#include <gtest/gtest.h>
#include <chesstastic_chess/ChessBoard.h>
#include <algorithm>

// Test for getBoard() for an empty map.
TEST(ChessBoardTest, EmptyGetBoard) {
    std::map<Position, ChessPiece> map;
    ChessBoard board = ChessBoard(map);
    EXPECT_EQ(board.get_board(), map);
    map.insert(std::pair<Position, ChessPiece>(Position(1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    EXPECT_NE(board.get_board(), map);
}

// Test for getBoard() for some entries in map.
TEST(ChessBoardTest, NotEmptyGetBoard){
    std::map<Position, ChessPiece> map;
    map.insert(std::pair<Position, ChessPiece>(Position(55), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::QUEEN)));
    ChessBoard board1 = ChessBoard(map);
    EXPECT_EQ(board1.get_board(), map);
    map.insert(std::pair<Position, ChessPiece>(Position(45), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING)));
    EXPECT_NE(board1.get_board(), map);
    ChessBoard board2 = ChessBoard(map);
    EXPECT_EQ(board2.get_board(), map);
}

// Test for setField(Position, ChessPiece) for some trivial <Position, ChessPiece> pairs.
TEST(ChessBoardTest, TrivialSetField) {
    std::map<Position, ChessPiece> map;
    ChessBoard board1 = ChessBoard(map);
    board1.set_field(Position(13), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_NE(map, board1.get_board());
    map.insert(std::pair<Position, ChessPiece>
            (Position(13), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    EXPECT_EQ(map, board1.get_board());
    board1.set_field(Position(46), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT));
    EXPECT_NE(map, board1.get_board());
    map.insert(std::pair<Position, ChessPiece>
                       (Position(46), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    EXPECT_EQ(map, board1.get_board());
}

// Test for isEmptyBoard() where the supplied board is empty.
TEST(ChessBoardTest, EmptyIsEmptyBoard) {
    std::map<Position,ChessPiece> map;
    ChessBoard board = ChessBoard(map);
    EXPECT_TRUE(board.is_board_empty());
}

// Test for isEmptyBoard() where the supplied board is not empty.
TEST(ChessBoardTest, NotEmptyIsEmptyBoard) {
    std::map<Position, ChessPiece> map;
    ChessBoard board = ChessBoard(map);
    board.set_field(Position(4), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    EXPECT_FALSE(board.is_board_empty());
}

// Test for isFieldEmpty(Position) where the supplied Position is empty.
TEST(ChessBoardTest, EmptyIsFieldEmpty) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    EXPECT_TRUE(board.is_field_empty(Position(1)));
    board.set_field(Position(2), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    board.set_field(Position(0), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));
    EXPECT_TRUE(board.is_field_empty(Position(1)));
}

// Test for isFieldEmpty(Position) where the supplied Position is not empty.
TEST(ChessBoardTest, NotEmptyIsFieldEmpty) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT));
    EXPECT_FALSE(board.is_field_empty(Position(1)));
}

// Test for getLocationOf(ChessPiece) where the supplied ChessPiece is somewhere on the board.
TEST(ChessBoardTest, OnBoardGetLocationOf) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT));
    board.set_field(Position(2), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_EQ(1, board.get_location_of(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT)).size());
    EXPECT_EQ(1, board.get_location_of(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN)).size());
    EXPECT_EQ(Position(1), board.get_location_of(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT)).front());
    EXPECT_EQ(Position(2), board.get_location_of(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN)).front());
}

// Test for getLocationOf(ChessPiece) where the supplied ChessPiece is not on the board.
TEST(ChessBoardTest, NotOnBoardGetLocationOf) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KNIGHT));
    board.set_field(Position(2), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_EQ(0, board.get_location_of(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING)).size());
    EXPECT_EQ(0, board.get_location_of(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT)).size());
    EXPECT_EQ(0, board.get_location_of(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN)).size());
}

// Test for getLocationOf(ChessPiece) where the supplied ChessPiece is on the board more than once.
TEST(ChessBoardTest, MultipleOnBoardGetLocationOf) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(2), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(3), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    board.set_field(Position(4), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));

    std::vector<Position> result = board.get_location_of(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    std::vector<Position> checkResult = {Position(1), Position(2), Position(3), Position(4)};

    std::sort(result.begin(), result.end());
    std::sort(checkResult.begin(), checkResult.end());

    EXPECT_EQ(4, result.size());
    EXPECT_EQ(checkResult, result);
}

// Test for RemoveAt(Position) where a piece is on the supplied position on the board.
TEST(ChessBoardTest, FullRemoveAt) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_FALSE(board.is_field_empty((Position(1))));
    board.remove_at(Position(1));
    EXPECT_TRUE(board.is_field_empty(Position(1)));
}

// Test for RemoveAt(Position) where no piece is on the supplied position on the board.
TEST(ChessBoardTest, EmptyRemoveAt) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    EXPECT_TRUE(board.is_field_empty((Position(1))));
    board.remove_at(Position(1));
    EXPECT_TRUE(board.is_field_empty(Position(1)));
}

// Test for getField(Position) where a piece is on the supplied position on the board.
TEST(ChessBoardTest, FullGetField) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    board.set_field(Position(1), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_EQ(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), board.get_field(Position(1)));
    board.set_field(Position(1), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    EXPECT_NE(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN), board.get_field(Position(1)));
    EXPECT_EQ(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN), board.get_field(Position(1)));
}

// Test for getField(Position) where no piece is on the supplied position on the board.
TEST(ChessBoardTest, EmptyGetField) {
    ChessBoard board = ChessBoard(std::map<Position, ChessPiece>());
    EXPECT_FALSE(board.get_field(Position(1)).has_value());
    board.set_field(Position(2), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    board.set_field(Position(0), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));
    EXPECT_FALSE(board.get_field(Position(1)).has_value());
}

// Test for makeMove(ChessMove) where the move is of MoveType::REGULAR.
TEST(ChesSBoardTest, RegularMove) {
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

    EXPECT_EQ(board.get_field(Position(0, 1)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(0, 2)));
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                Position(0, 1), Position(0, 2), MoveType::REGULAR);
    board.make_move(move1);
    EXPECT_EQ(board.get_field(Position(0, 2)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(0, 1)));

    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                Position(0,6), Position(0,2), MoveType::REGULAR);
    board.make_move(move2);
    EXPECT_TRUE(board.is_field_empty(Position(0, 6)));
    EXPECT_EQ(board.get_field(Position(0, 2)), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
}

// Test for makeMove(ChessMove) where the move is of MoveType:REGULAR and the move is en passant.
TEST(ChessBoardTest, RegularMoveEnPassant) {
    std::map<Position, ChessPiece> startPosition;

    // En passant move for black
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 3), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 3), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    // En passant move for white
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 4), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 4), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::PAWN)));

    ChessBoard board = ChessBoard(startPosition);

    EXPECT_EQ(board.get_field(Position(1, 3)), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    EXPECT_EQ(board.get_field(Position(0, 3)), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(0, 2)));
    ChessMove blackEnPassant = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN),
                                         Position(1, 3), Position(0, 2), MoveType::REGULAR);
    board.make_move(blackEnPassant);
    EXPECT_EQ(board.get_field(Position(0, 2)), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(1, 3)));
    EXPECT_TRUE(board.is_field_empty(Position(0, 3)));

    EXPECT_EQ(board.get_field(Position(1, 4)), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::PAWN));
    EXPECT_EQ(board.get_field(Position(0, 4)), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(1, 5)));
    ChessMove whiteEnPassant = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                         Position(0, 4), Position(1, 5), MoveType::REGULAR);
    board.make_move(whiteEnPassant);
    EXPECT_EQ(board.get_field(Position(1, 5)), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(1, 4)));
    EXPECT_TRUE(board.is_field_empty(Position(0, 4)));
}

// Test for makeMove(ChessMove) where the move is of MoveType::CASTLE_LONG or MoveType::CASTLE_SHORT.
TEST(ChessBoardTest, CastleMove) {
    std::map<Position, ChessPiece> startPosition;

    // Arranging the pieces for castling long.
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(4, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KING)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(5, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::BISHOP)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(6, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::KNIGHT)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 0), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::ROOK)));

    // Arranging the pieces for castling short.
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
    startPosition.insert(std::pair<Position, ChessPiece>(Position(7, 7), ChessPiece(
            ChessPieceSide::BLACK, ChessPieceType::ROOK)));

    ChessBoard board = ChessBoard(startPosition);


    EXPECT_EQ(board.get_field(Position(0, 0)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    EXPECT_EQ(board.get_field(Position(4, 0)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING));
    EXPECT_TRUE(board.is_field_empty(Position(3, 0)));
    EXPECT_TRUE(board.is_field_empty(Position(2, 0)));
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING),
                                Position(0, 0), Position(0, 0), MoveType::CASTLE_LONG);
    board.make_move(move1);
    EXPECT_EQ(board.get_field(Position(2, 0)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KING));
    EXPECT_EQ(board.get_field(Position(3, 0)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    EXPECT_TRUE(board.is_field_empty(Position(0, 0)));
    EXPECT_TRUE(board.is_field_empty(Position(4, 0)));

    EXPECT_EQ(board.get_field(Position(7, 7)).value(), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    EXPECT_EQ(board.get_field(Position(4, 7)).value(), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING));
    EXPECT_TRUE(board.is_field_empty(Position(5, 7)));
    EXPECT_TRUE(board.is_field_empty(Position(6, 7)));
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING),
                                Position(0, 0), Position(0, 0), MoveType::CASTLE_SHORT);
    board.make_move(move2);
    EXPECT_EQ(board.get_field(Position(6, 7)).value(), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::KING));
    EXPECT_EQ(board.get_field(Position(5, 7)).value(), ChessPiece(ChessPieceSide::BLACK, ChessPieceType::ROOK));
    EXPECT_TRUE(board.is_field_empty(Position(4, 7)));
    EXPECT_TRUE(board.is_field_empty(Position(7, 7)));
}

// Test for makeMove(ChessMove) where the move is of MoveType::PROMOTION_ROOK, MoveType::PROMOTION_KNIGHT,
// MoveType::PROMOTION_BISHOP or MoveType::PROMOTION_QUEEN.
TEST(ChesSBoardTest, PromotionMove) {
    std::map<Position, ChessPiece> startPosition;

    // Placing pawns on the board.
    startPosition.insert(std::pair<Position, ChessPiece>(Position(0, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(1, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(2, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));
    startPosition.insert(std::pair<Position, ChessPiece>(Position(3, 6), ChessPiece(
            ChessPieceSide::WHITE, ChessPieceType::PAWN)));

    ChessBoard board = ChessBoard(startPosition);

    EXPECT_EQ(board.get_field(Position(0, 6)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(0, 7)));
    ChessMove move1 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                Position(0, 6), Position(0, 7), MoveType::PROMOTION_ROOK);
    board.make_move(move1);
    EXPECT_EQ(board.get_field(Position(0, 7)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::ROOK));
    EXPECT_TRUE(board.is_field_empty(Position(0, 6)));

    EXPECT_EQ(board.get_field(Position(1, 6)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(1, 7)));
    ChessMove move2 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                Position(1, 6), Position(1, 7), MoveType::PROMOTION_KNIGHT);
    board.make_move(move2);
    EXPECT_EQ(board.get_field(Position(1, 7)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::KNIGHT));
    EXPECT_TRUE(board.is_field_empty(Position(1, 6)));

    EXPECT_EQ(board.get_field(Position(2, 6)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(2, 7)));
    ChessMove move3 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                Position(2, 6), Position(2, 7), MoveType::PROMOTION_BISHOP);
    board.make_move(move3);
    EXPECT_EQ(board.get_field(Position(2, 7)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::BISHOP));
    EXPECT_TRUE(board.is_field_empty(Position(2, 6)));

    EXPECT_EQ(board.get_field(Position(3, 6)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN));
    EXPECT_TRUE(board.is_field_empty(Position(3, 7)));
    ChessMove move4 = ChessMove(ChessPiece(ChessPieceSide::WHITE, ChessPieceType::PAWN),
                                Position(3, 6), Position(3, 7), MoveType::PROMOTION_QUEEN);
    board.make_move(move4);
    EXPECT_EQ(board.get_field(Position(3, 7)).value(), ChessPiece(ChessPieceSide::WHITE, ChessPieceType::QUEEN));
    EXPECT_TRUE(board.is_field_empty(Position(3, 6)));
}
