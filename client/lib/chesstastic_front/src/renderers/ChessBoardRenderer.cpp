//
// Created by Dawid Kulikowski on 12/10/2021.
//

#include <chesstastic_chess/ChessBoard.h>
#include "ChessBoardRenderer.h"

void ChessBoardRenderer::render(QWidget *canvas, QPainter &painter, const QPaintEvent *evt, ChessBoard board,
                                ChessPieceSide ignore) {
    this->render_squares(canvas, painter, board, ignore);
}

void ChessBoardRenderer::render_squares(QWidget *canvas, QPainter &painter, ChessBoard board, ChessPieceSide ignore) {
    const auto height = canvas->height();
    const auto width = canvas->width();

    const auto size_of_square_y = height / SIZE_OF_CHESS_BOARD;
    const auto size_of_square_x = width / SIZE_OF_CHESS_BOARD;
    const auto ideal_square_size =
            size_of_square_x < size_of_square_y ? size_of_square_x : size_of_square_y; // Take smallest
    // How much space we'll have leftover on each side
    const auto offset_y = (height - (ideal_square_size * 8)) / 2;
    const auto offset_x = (width - (ideal_square_size * 8)) / 2;

    QTransform transform;

    transform.translate(offset_x, offset_y);
    painter.save();
    painter.setWorldTransform(transform, true);


    for (int y = 0; y < SIZE_OF_CHESS_BOARD; y++) {
        for (int x = 0; x < SIZE_OF_CHESS_BOARD; x++) {
            auto colour = (x + y) % 2 ? Qt::GlobalColor::white : Qt::GlobalColor::black;

            auto rect_y = y * ideal_square_size;
            auto rect_x = x * ideal_square_size;

            QRect rect(QPoint(rect_x, rect_y), QSize(ideal_square_size, ideal_square_size));
            painter.fillRect(rect, colour);
            auto idx = qt_coords_to_chess_coords(x, y);

            if (!board.is_board_empty() && !board.is_field_empty(idx) && board.get_field(idx)->get_side() != ignore) {
                auto text = board.get_field(idx)->get_unicode();
                painter.save();

                QFont font = painter.font();
                font.setPixelSize(100);
                painter.setFont(font);

                painter.setPen({255, 105, 180});

                painter.translate(rect_x + (ideal_square_size / 2), rect_y + (ideal_square_size / 2));
                painter.rotate(-90);
                painter.translate(-ideal_square_size / 2, -ideal_square_size / 2);

                QRect zero_sqr(QPoint(0, 0), QSize(ideal_square_size, ideal_square_size));
                painter.drawText(zero_sqr, Qt::AlignCenter, QString::fromUtf8(text.c_str()));
                painter.restore();
            }
        }
    }

    painter.setPen(QPen(QBrush(Qt::green), 5));
    painter.drawRect(0, 0, ideal_square_size * SIZE_OF_CHESS_BOARD,
                     ideal_square_size * SIZE_OF_CHESS_BOARD);

    painter.restore();
}

int ChessBoardRenderer::qt_coords_to_chess_coords(int x, int y) {
    int new_x = SIZE_OF_CHESS_BOARD - 1 - y; // 8 - 0 = 8
    int new_y = SIZE_OF_CHESS_BOARD - 1 - x; // 8 - 0 = 8

    return new_y * SIZE_OF_CHESS_BOARD + new_x;
}

