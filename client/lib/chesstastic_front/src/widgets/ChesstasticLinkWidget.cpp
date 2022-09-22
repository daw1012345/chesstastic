//
// Created by Dawid Kulikowski on 21/10/2021.
//

#include "ChesstasticLinkWidget.h"
#include "ChesstasticAPIService.h"
#include <QVBoxLayout>

ChesstasticLinkWidget::ChesstasticLinkWidget(std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> api,
                                             QWidget *parent) : QWidget(
        parent), controller(api) {
    QObject::connect(&controller, &ChesstasticAPILinkingMiddleman::linked, this, &ChesstasticLinkWidget::didLink);
    info = new QLabel();
    token = new QLineEdit();
    token->setReadOnly(true);
    info->setText("Your unique link code:");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(info);
    layout->addWidget(token, 0);
    this->setLayout(layout);
}

void ChesstasticLinkWidget::showEvent(QShowEvent *ev) {
    auto token = controller.start();
    this->token->setText(QString::fromStdString(token));
    QWidget::showEvent(ev);
}

void ChesstasticLinkWidget::didLink() {
    this->token->setText("Done!");
    this->info->setText("Your board is linked!");
}