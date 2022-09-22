//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSPIECE_H
#define CHESSTASTIC_CHESSPIECE_H

#include <string>
#include <vector>
#include "Position.h"

/** This enum class contains all the colors the ChessPieces can be.
 */
enum class ChessPieceSide {
    BLACK, WHITE
};

/** This enum class contains all the types a ChessPiece can be.
 */
enum class ChessPieceType {
    KING, ROOK, BISHOP, QUEEN, KNIGHT, PAWN
};

struct ChessPiece {
private:
    ChessPieceSide color;
    ChessPieceType piece;

public:
    bool operator==(const ChessPiece &other) const;
    bool operator!=(const ChessPiece &other) const;

    /** This constructor for a ChessPiece takes a ChessPieceSide and a ChessPieceType to create a ChessPiece.
     * @param side The color of the ChessPiece
     * @param type The type of the ChessPiece
     */
    ChessPiece(ChessPieceSide side, ChessPieceType type);
    /** This constructor for a ChessPiece takes a string color and a string type to create a ChessPiece
     * @param sside The string white or black
     * @param type The string type the ChessPiece has to become
     */
    ChessPiece(std::string side, std::string type);

    /** This function can be used to get the type of the ChessPiece.
     * @return The enum ChessPieceType that belongs to this ChessPiece
     */
    ChessPieceType get_type();
    /** This function can be used to get the color of the ChessPiece.
     * @return The enum ChessPieceSide that belongs to this ChessPiece
     */
    ChessPieceSide get_side();
    /** This function can be used to get the name of the ChessPiece which is the ChessNotation
     * @return The chessNotation name of the ChessPiece
     */
    std::string get_name();
    /** This function will return all the possible Positions the ChessPiece is able to get on given a position,
     * The function does not take other Pieces into account.
     * @param position The Positions of the ChessPiece
     * @param piece The ChessPiece itself
     * @return A vector which contains all the possible positions the ChessPiece is able to reach
     */
    static std::vector<Position> get_regular_moves(Position position, ChessPiece piece);

    static std::vector<Position> remove_invalid_moves(std::vector<Position> positions);

    std::string get_unicode() {
        switch (piece) {
            case ChessPieceType::KING:
                return u8"\u2654";
            case ChessPieceType::ROOK:
                return u8"\u2656";
            case ChessPieceType::BISHOP:
                return u8"\u2657";
            case ChessPieceType::QUEEN:
                return u8"\u2655";
            case ChessPieceType::KNIGHT:
                return u8"\u2658";
            case ChessPieceType::PAWN:
                return u8"\u2659";
        }
    }
};


#endif //CHESSTASTIC_CHESSPIECE_H
