//
// Created by Dawid Kulikowski on 02/11/2021.
//

#include "ChessStatusWidget.h"
#include "ChessPlayerStatusWidget.h"
#include <QVBoxLayout>
#include <QSpacerItem>

ChessStatusWidget::ChessStatusWidget(QWidget* parent): QWidget(parent) {
    setMinimumSize(200, 80);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    auto layout = new QVBoxLayout;
    auto p1 = new ChessPlayerStatusWidget(ChessPieceSide::BLACK);
    auto p2 = new ChessPlayerStatusWidget(ChessPieceSide::WHITE);

    layout->addWidget(p1);
    layout->addStretch(2);
    layout->addWidget(p2);
    setLayout(layout);
}

void ChessStatusWidget::poll_status_changed(bool) {
    std::cout << "[STATUS] Poll status changed!" << std::endl;
}

void ChessStatusWidget::auth_status_changed(bool, std::optional<std::string>) {
    std::cout << "[STATUS] Auth status changed!" << std::endl;
}
