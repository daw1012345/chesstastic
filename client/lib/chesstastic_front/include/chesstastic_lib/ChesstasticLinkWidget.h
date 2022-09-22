//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICLINKWIDGET_H
#define CHESSTASTIC_CHESSTASTICLINKWIDGET_H

#include <QWidget>
#include "ChesstasticAPILinkingMiddleman.h"
#include "ChesstasticAPIService.h"
#include <QLineEdit>
#include <QLabel>

class ChesstasticLinkWidget : public QWidget {
Q_OBJECT
public:
    explicit ChesstasticLinkWidget(std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService>, QWidget * = nullptr);

private:
    ChesstasticAPILinkingMiddleman controller;
    QLabel *info;
    QLineEdit *token;
public slots:

    void showEvent(QShowEvent *ev) override;

    void didLink();
};


#endif //CHESSTASTIC_CHESSTASTICLINKWIDGET_H
