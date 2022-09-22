//
// Created by Dawid Kulikowski on 03/11/2021.
//

#ifndef CHESSTASTIC_CHESSPLAYERSTATUSWIDGET_H
#define CHESSTASTIC_CHESSPLAYERSTATUSWIDGET_H

#include <QWidget>
#include <chesstastic_chess/ChessBoard.h>
#include <QLabel>
#include <QLineEdit>

#include "chesstastic_chess/ChessPiece.h"
#include "ChessGame.h"

class ChessPlayerStatusWidget : public QWidget {
Q_OBJECT
public:
    explicit ChessPlayerStatusWidget(ChessPieceSide, QWidget * = nullptr);

public slots:

    void player_pair_changed(std::map<ChessPieceSide, std::string>);

    void board_changed(ChessBoard);

    void move_requested(ChessPieceSide);

    void game_end(std::optional<ChessPieceSide>);

    void game_poll_start();


private:
    QLabel *user_name;
    QLineEdit *pieces_taken;
    ChessPieceSide side;
};


#endif //CHESSTASTIC_CHESSPLAYERSTATUSWIDGET_H
