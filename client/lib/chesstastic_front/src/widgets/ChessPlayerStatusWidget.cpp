//
// Created by Dawid Kulikowski on 03/11/2021.
//

#include "ChessPlayerStatusWidget.h"
#include <QVBoxLayout>
#include <QLabel>

ChessPlayerStatusWidget::ChessPlayerStatusWidget(ChessPieceSide side, QWidget* parent): QWidget(parent), side(side) {
    auto layout = new QVBoxLayout;
    this->user_name = new QLabel;
    this->pieces_taken = new QLineEdit;
    this->pieces_taken->setReadOnly(true);
    if (side == ChessPieceSide::BLACK) {
        this->user_name->setText("(Black)");
    } else {
        this->user_name->setText("(White)");
    }

    auto palette = this->user_name->palette();
    palette.setColor(this->user_name->foregroundRole(), Qt::blue);
    this->user_name->setPalette(palette);

    layout->addStretch();
    layout->addWidget(user_name, 1);
    layout->addWidget(pieces_taken, 2);

    this->setMinimumSize(100, 50);

    setLayout(layout);
}

void ChessPlayerStatusWidget::board_changed(ChessBoard board) {
    auto missing_pieces = this->side == ChessPieceSide::BLACK ? board.get_missing_pieces(ChessPieceSide::WHITE) : board.get_missing_pieces(ChessPieceSide::BLACK);

    std::string missing_str;

    for (auto [piece, missing_n] : missing_pieces) {
        if (missing_n == 0) continue;

        missing_str += missing_str.empty() ? ChessPiece(this->side, piece).get_name() : " " + ChessPiece(this->side, piece).get_name();
    }

    this->pieces_taken->setText(QString::fromStdString(missing_str));
}

void ChessPlayerStatusWidget::move_requested(ChessPieceSide req_side) {
    if (req_side == this->side) {
        auto palette = this->user_name->palette();
        palette.setColor(this->user_name->foregroundRole(), Qt::yellow);
        this->user_name->setPalette(palette);
    } else {
        auto palette = this->user_name->palette();
        palette.setColor(this->user_name->foregroundRole(), Qt::blue);
        this->user_name->setPalette(palette);
    }

}

void ChessPlayerStatusWidget::game_end(std::optional<ChessPieceSide> winner) {
    if (this->side == winner || !winner.has_value()) {
        auto palette = this->user_name->palette();
        palette.setColor(this->user_name->foregroundRole(), Qt::green);
        this->user_name->setPalette(palette);
    }
}

void ChessPlayerStatusWidget::game_poll_start() {
    if (side == ChessPieceSide::BLACK) {
        this->user_name->setText("(Black)");
    } else {
        this->user_name->setText("(White)");
    }
}

void ChessPlayerStatusWidget::player_pair_changed(std::map<ChessPieceSide, std::string> players) {
    auto this_name = players.at(this->side);
    this->user_name->setText(
            QString::fromStdString(this_name + " (" + (this->side == ChessPieceSide::BLACK ? "Black" : "White") + ")"));
}
