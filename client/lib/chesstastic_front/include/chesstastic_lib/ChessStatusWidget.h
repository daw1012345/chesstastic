//
// Created by Dawid Kulikowski on 02/11/2021.
//

#ifndef CHESSTASTIC_CHESSSTATUSWIDGET_H
#define CHESSTASTIC_CHESSSTATUSWIDGET_H
#include <QWidget>
#include "ChessGame.h"

class ChessStatusWidget: public QWidget {
    Q_OBJECT
public:
    ChessStatusWidget(QWidget* = nullptr);
public slots:
    void poll_status_changed(bool);
    void auth_status_changed(bool, std::optional<std::string>);
};


#endif //CHESSTASTIC_CHESSSTATUSWIDGET_H
