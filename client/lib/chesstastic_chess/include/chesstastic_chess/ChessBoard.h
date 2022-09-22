//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSBOARD_H
#define CHESSTASTIC_CHESSBOARD_H
#include <optional>
#include <map>
#include <vector>
#include "ChessPiece.h"
#include "Position.h"
#include "ChessMove.h"

class ChessBoard {
private:
    std::map<Position, ChessPiece> board;

public:
    /** This method will create an instance of the class ChessBoard.
     */
    ChessBoard(std::map<Position, ChessPiece> = {});

public:
    /** This method will check if the board does not contain any chess pieces.
     * @return True if the board is empty, False otherwise
     */
    bool is_board_empty() const;
    /** This method will check if a Position does not contain a chess piece.
     * @param
     * @return True if the Position is empty, False otherwise
     */
    bool is_field_empty(Position position) const;
    /** This function will set a chess piece on a position.
     * @param position The position where the piece has to come
     * @param piece The ChessPiece to be places on that position
     */
    void set_field(Position position, ChessPiece piece);
    /** This function will return the ChessPiece that is located on position, or does not return anything if empty.
     * @param position The position to be checked
     * @return The ChessPiece on the position specified
     */
    std::optional<ChessPiece> get_field(Position position) const;
    /** This funtion will give you a representation of the board.
     * @return A map which contains all Locations which contain a ChessPiece and which ChessPiece
     */
    std::map<Position, ChessPiece> get_board() const;
    /** This function can be used to find the location of a ChessPiece,
     * in the case of multiple ChessPieces, it will return multiple locations.
     * @param piece The ChessPiece of which you want to know the location
     * @return A vector which contains all the Locations with the given ChessPiece
     */
    std::vector<Position> get_location_of(ChessPiece piece);
    /** This function will remove a Position and corresponding ChessPiece from the board.
     * @param position The position that has to be removed from the board
     * @return The removed ChessPiece which belonged to that location, or nothing if location did not contain any
     */
    std::optional<ChessPiece> remove_at(Position position);
    /** This function can be used to make a move on a given board.
     * @param board The board to make the move on
     * @param move The ChessMove that has to be performed
     * @return The given ChessBoard after the move has been performed
     */
    static std::map<Position, ChessPiece> make_move(ChessBoard board, ChessMove move);
    /** This function can be used to make a move on this board.
     * @param move The ChessMove to be performed
     */
    void make_move(ChessMove move);

    std::map<ChessPieceType, int> get_missing_pieces(ChessPieceSide color);

private:
    /** This function will perform the move on the board and the ChessPiece will get the specified type.
     * @param move The move to be performed on the board
     * @param newType The ChessPieceType the ChessPiece has to get
     */
    void do_move(ChessMove move, ChessPieceType newType);
};


#endif //CHESSTASTIC_CHESSBOARD_H
