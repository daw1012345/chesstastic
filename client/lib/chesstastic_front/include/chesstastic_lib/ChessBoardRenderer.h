//
// Created by Dawid Kulikowski on 12/10/2021.
//

#ifndef CHESSTASTIC_CHESSBOARDRENDERER_H
#define CHESSTASTIC_CHESSBOARDRENDERER_H

#include <QWidget>
#include <QPainter>
#include "chesstastic_chess/ChessBoard.h"


class ChessBoardRenderer {
public:
    static const int SIZE_OF_CHESS_BOARD = 8;

    void render(QWidget *, QPainter &, const QPaintEvent *, ChessBoard, ChessPieceSide);

private:
    int qt_coords_to_chess_coords(int, int);

    void render_squares(QWidget *canvas, QPainter &painter, ChessBoard board, ChessPieceSide ignore);
};


#endif //CHESSTASTIC_CHESSBOARDRENDERER_H
