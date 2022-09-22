//
// Created by Dawid Kulikowski on 17/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICWINDOW_H
#define CHESSTASTIC_CHESSTASTICWINDOW_H

#include <QMainWindow>
#include "ChesstasticSettingsWindow.h"
#include "ChesstasticAPILinkingMiddleman.h"
#include "ChesstasticAPIMiddleman.h"
#include "ChessGame.h"
#include "ChesstasticAPIService.h"
#include "ChesstasticLinkWidget.h"
#include "ChessBoardWidget.h"
#include "ChessStatusWidget.h"
#include "ChessPlayerStatusWidget.h"

class ChesstasticWindow : public QMainWindow {
Q_OBJECT
public:
    ChesstasticWindow(std::weak_ptr<CameraHandler> handler, std::shared_ptr<CameraFeedback> feedback,
                      std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService>);

private:
    ChesstasticSettingsWindow settingsWindow;
    ChesstasticAPIMiddleman gameSearchController;
    ChesstasticLinkWidget linkWindow;
    ChessBoardWidget *board;

    ChessPlayerStatusWidget *white_status;
    ChessPlayerStatusWidget *black_status;


    std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> apiService;
    std::shared_ptr<CameraFeedback> feedback;
    std::unique_ptr<ChessGame> game;

protected slots:

// Poll succeeds
    void new_game(int);

// Menu actions
    void settings_requested();
    void open_link();
    void unlink();
    void start_game();

};


#endif //CHESSTASTIC_CHESSTASTICWINDOW_H
