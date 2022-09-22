//
// Created by degra on 01/11/2021.
//

#ifndef CHESSTASTIC_CHESSMOVE_H
#define CHESSTASTIC_CHESSMOVE_H
#include "ChessPiece.h"
#include "Position.h"

enum class MoveType {
    REGULAR,
    CASTLE_LONG,
    CASTLE_SHORT,
    PROMOTION_BISHOP,
    PROMOTION_QUEEN,
    PROMOTION_KNIGHT,
    PROMOTION_ROOK,
    RESIGNATION
};

class ChessMove {
public:
    /** This function will create an instance of the class ChessMove.
     * @param piece The piece that is supposed to move
     * @param source The original location of the piece on the board
     * @param destination The Position where the piece is going to be moved
     * @param type The Type of move which is one of the enum values MoveType
     */
    ChessMove(ChessPiece piece, Position source, Position destination, MoveType type);
    /** This function will return the original location of the move.
     * @return The original location of the move
     */
    Position getSource() const;
    /** This function will return the destination of the move.
     * @return The Position where the piece will end up
     */
    Position getDestination() const;
    /** This function will return the Type of move that is performed.
     * @return move type
     */
    MoveType getType() const;
    /** This function will return the chessPiece that with which the move is performed with.
     * @return ChessPiece
     */
    ChessPiece getPiece() const;
    /** This method will set the source Position to a new source Position.
     * @param source The new source Position
     */
    void setSource(Position source);
    /** This method will set the destination of the move to new destination.
     * @param destination The Position which is the new destination
     */
    void setDestination(Position destination);
    /** This method will set the type of the move to a new type
     * @param type the type of the move
     */
    void setType(MoveType type);

    bool operator==(ChessMove move2) const;
    bool operator!=(ChessMove move2) const;
    bool operator<(ChessMove move2) const;
    bool operator>(ChessMove move2) const;
private:
    ChessPiece piece;
    Position source;
    Position destination;
    MoveType type;
};



#endif //CHESSTASTIC_CHESSMOVE_H
