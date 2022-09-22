//
// Created by Dawid Kulikowski on 24/09/2021.
//

#ifndef CHESSTASTIC_CHESSBOARDWIDGET_H
#define CHESSTASTIC_CHESSBOARDWIDGET_H

#include <QWidget>
#include "ChessBoardRenderer.h"
#include "chesstastic_chess/ChessPiece.h"

class ChessBoardWidget : public QWidget {
Q_OBJECT
public:
    explicit ChessBoardWidget(QWidget *parent = nullptr);

public slots:

    void update_board_repr(ChessBoard b, ChessPieceSide side) {
        this->recentBoard = b;
        this->ignore = side;
        this->repaint();
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    ChessBoard recentBoard;
    ChessBoardRenderer renderer;

    ChessPieceSide ignore;

};

#endif //CHESSTASTIC_CHESSBOARDWIDGET_H
