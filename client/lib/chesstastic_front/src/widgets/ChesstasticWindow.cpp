//
// Created by Dawid Kulikowski on 17/10/2021.
//

#include "ChesstasticWindow.h"
#include <QMenuBar>
#include <QWindow>
#include <ChesstasticAPIService.h>
#include <ChessBoardWidget.h>
#include "ChessStatusWidget.h"

ChesstasticWindow::ChesstasticWindow(std::weak_ptr<CameraHandler> handler, std::shared_ptr<CameraFeedback> feedback,
                                     std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> apiService)
        : QMainWindow(), settingsWindow(handler),
          gameSearchController(apiService), apiService(apiService), linkWindow(apiService), feedback(feedback) {
    auto menu = this->menuBar()->addMenu(tr("&Chesstastic"));
    auto gmenu = this->menuBar()->addMenu(tr("&Game"));

    QAction *openSettings = new QAction;
    openSettings->setText("Settings");

    QAction *startLink = new QAction;
    startLink->setText("Link...");
    menu->addAction(startLink);

    QAction *startUNLink = new QAction;
    startUNLink->setText("Unlink");
    menu->addAction(startUNLink);

    QAction *startGame = new QAction;
    startGame->setText("Start Game");
    gmenu->addAction(startGame);

    menu->addAction(openSettings);
    menu->addAction(startLink);
    menu->addAction(startUNLink);

    auto c_widget = new QWidget;
    auto layout = new QHBoxLayout;
    ChessBoardWidget *board = new ChessBoardWidget;
    this->board = board;

    this->white_status = new ChessPlayerStatusWidget(ChessPieceSide::WHITE);
    this->black_status = new ChessPlayerStatusWidget(ChessPieceSide::BLACK);
    auto left_status = new QVBoxLayout;
    auto right_status = new QVBoxLayout;

    left_status->addStretch(2);
    left_status->addWidget(black_status);

    right_status->addWidget(white_status);
    right_status->addStretch(2);
    layout->addLayout(left_status);
    layout->addWidget(board, 1);
    layout->addLayout(right_status);

    c_widget->setLayout(layout);
    c_widget->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::MinimumExpanding);
    this->setCentralWidget(c_widget);

    QObject::connect(openSettings, &QAction::triggered, this, &ChesstasticWindow::settings_requested);
    QObject::connect(startLink, &QAction::triggered, this, &ChesstasticWindow::open_link);
    QObject::connect(startUNLink, &QAction::triggered, this, &ChesstasticWindow::unlink);
    QObject::connect(startGame, &QAction::triggered, this, &ChesstasticWindow::start_game);
    QObject::connect(&gameSearchController, &ChesstasticAPIMiddleman::new_game, this, &ChesstasticWindow::new_game);
}

void ChesstasticWindow::settings_requested() {
    settingsWindow.showNormal();
}

void ChesstasticWindow::unlink() {
    apiService->reset_auth();
}

void ChesstasticWindow::start_game() {
    this->gameSearchController.start_poll();
}

void ChesstasticWindow::open_link() {
    this->linkWindow.show();
}

void ChesstasticWindow::new_game(int id) {
    this->game = std::make_unique<ChessGame>(id, feedback, apiService);
    QObject::connect(game.get(), &ChessGame::board_changed, this->board, &ChessBoardWidget::update_board_repr);

    QObject::connect(game.get(), &ChessGame::board_changed, this->white_status,
                     &ChessPlayerStatusWidget::board_changed);
    QObject::connect(game.get(), &ChessGame::board_changed, this->black_status,
                     &ChessPlayerStatusWidget::board_changed);
    QObject::connect(game.get(), &ChessGame::next_turn, this->white_status, &ChessPlayerStatusWidget::move_requested);
    QObject::connect(game.get(), &ChessGame::next_turn, this->black_status, &ChessPlayerStatusWidget::move_requested);
    QObject::connect(game.get(), &ChessGame::game_end, this->white_status, &ChessPlayerStatusWidget::game_end);
    QObject::connect(game.get(), &ChessGame::game_end, this->black_status, &ChessPlayerStatusWidget::game_end);
    QObject::connect(game.get(), &ChessGame::player_pair_changed, this->white_status,
                     &ChessPlayerStatusWidget::player_pair_changed);
    QObject::connect(game.get(), &ChessGame::player_pair_changed, this->black_status,
                     &ChessPlayerStatusWidget::player_pair_changed);

    this->game->start();
}
