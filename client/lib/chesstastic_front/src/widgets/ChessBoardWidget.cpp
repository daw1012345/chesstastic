//
// Created by Dawid Kulikowski on 24/09/2021.
//

#include "ChessBoardWidget.h"
#include <iostream>
#include <QTimer>
#include <QPainter>

void ChessBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    this->renderer.render(this, painter, event, recentBoard, ignore);
}

ChessBoardWidget::ChessBoardWidget(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(200, 200);

    this->renderer = ChessBoardRenderer();
}