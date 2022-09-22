//
// Created by degra on 01/11/2021.
//

#ifndef CHESSTASTIC_CHESSMOVEVALIDATIONSERVICE_H
#define CHESSTASTIC_CHESSMOVEVALIDATIONSERVICE_H
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "ChessMove.h"

class ChessMoveValidationService {
public:
    /** Will validate the given move with the board.
     * The move will be validated for correctness.
     * @param move The ChessMove to be validated
     * @return Will return True if the move is valid, False if not
     */
    bool isValidMove(ChessMove move, ChessBoard board);

    /** This function will return a list of all possible moves for a certain chessPiece and position and current board
     * @param piece The piece to be checked for
     * @param position The position of the piece to be checked for
     * @return A list with all the moves possible for this piece
     */
    std::vector<ChessMove> getPossibleMoves(ChessPiece piece, Position position, ChessBoard board);

private:
    /** Checks for a Pawn if the given move is a valid forward move.
     * @param move The move to be validated
     * @param dest The destination Position of the piece
     * @param source The source Position of the piece
     * @return True if it is a valid forward move, false otherwise
     */
    bool isValidForwardMovePawn(ChessMove move, Position dest, Position source, ChessBoard board);

    /** Checks for a Pawn if the given move is a valid attack move.
     * @param move The move to be validated
     * @param dest The destination of the Pawn
     * @param source The source Position of the Pawn
     * @param destinationPiece The piece that currently is on the destinationfield
     * @return True if it is a valid attackmove, False otherwise
     */
    bool isValidAttackMovePawn(ChessMove move, Position dest, Position source,
                               std::optional<ChessPiece> destinationPiece);

    /** Checks for a Pawn if the given move is a valid double move.
     * @param dest The destination of the Pawn
     * @param source The source Position of the Pawn
     * @param move The move to be validated
     * @return True if it is a valid double move, False otherwise
     */
    bool isValidDoubleMovePawn(Position dest, Position source, ChessMove move, ChessBoard board);

    /** Checks for a Pawn if the given move is a valid En Passant move.
     * @param move The move to be validated
     * @param source The source Position of the Pawn
     * @param dest The destination of the Pawn
     * @return True if it is a valid En Passant move, False otherwise
     */
    bool isValidEnPassantMovePawn(ChessMove move, Position source, Position dest, ChessBoard board);

    /** This method will check for a move with the move type Regular if it is a valid move.
     * @param move The move to be validated
     * @return True if the Regular move is valid, False otherwise
     */
    bool isValidRegularMove(ChessMove move, ChessBoard board);

    /** This method will check for a move with the move type Castle_Long if it is a valid move.
     * @param side This is the color of the player for which this is checked
     * @return True if it is a valid Castle_long, False otherwise
     */
    bool isValidCastleLong(ChessPieceSide side, ChessBoard board);

    /** This method will check for a move with the move type Castle_Short if it is a valid move.
     * @param side This is the color of the player for which this is checked
     * @return True if it is a valid Castle_Short, False otherwise
     */
    bool isValidCastleShort(ChessPieceSide side, ChessBoard board);

    /** This method will check for all the promotion types if it is a valid promotion.
     * @param move The move to be validated
     * @return True if it is a valid promotion, False otherwise
     */
    bool isValidPromotion(ChessMove move, ChessBoard board);

    /** This function checks if at least one of the list of positions is under attack by a piece of the attacker.
     * @param positions The list of Positions to be checked
     * @param attacker The color of the pieces that are checked to attack the Position
     * @return True if at least one of the positions is under attack by the attacker, False otherwise
     */
    bool aPositionUnderAttack(std::vector<Position> positions, ChessPieceSide attacker, ChessBoard board);

    /** This function checks if all the positions are under attack by a piece of the attacker.
     * @param positions The list of Positions to be checked
     * @param attacker The color of the pieces that are checked to attack the Position
     * @return True if all of the positions are under attack by the attacker, False otherwise
     */
    bool allPositionsUnderAttack(std::vector<Position> positions, ChessPieceSide attacker, ChessBoard board);

    /** This function will check for a list of Positions if it is empty
     * @param positions The list of positions to be checked
     * @return True if all positions are empty, False otherwise
     */
    bool emptySquares(std::vector<Position> positions, ChessBoard board);
};





#endif //CHESSTASTIC_CHESSMOVEVALIDATIONSERVICE_H
