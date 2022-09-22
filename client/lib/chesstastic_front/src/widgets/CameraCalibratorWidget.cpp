//
// Created by Dawid Kulikowski on 13/10/2021.
//

#include "CameraCalibratorWidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <opencv2/opencv.hpp>
#include <CameraImageRenderer.h>
#include <QIntValidator>
#include <QPainter>
#include <QTimer>

CameraCalibratorWidget::CameraCalibratorWidget(std::weak_ptr<CameraHandler> handler, QWidget *parent) : QWidget(parent),
                                                                                                        handler(handler) {
    QHBoxLayout *nlayout = new QHBoxLayout;
    QFormLayout *buttons = new QFormLayout;
    nlayout->setAlignment(Qt::AlignCenter);

    QLineEdit *text_topx = new QLineEdit();
    this->topx = text_topx;
    text_topx->setValidator(new QIntValidator());
    text_topx->setMaximumWidth(50);
    text_topx->setText("0.0");
    text_topx->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    buttons->addRow(tr("&Top (x)"), text_topx);

    QLineEdit *text_topy = new QLineEdit();
    this->topy = text_topy;
    text_topy->setValidator(new QIntValidator());
    text_topy->setMaximumWidth(50);
    text_topy->setText("0.0");
    text_topy->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    buttons->addRow(tr("&Top (y)"), text_topy);

    QLineEdit *text_botx = new QLineEdit();
    this->botx = text_botx;
    text_botx->setValidator(new QIntValidator());
    text_botx->setMaximumWidth(50);
    text_botx->setText("0.0");
    text_botx->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    buttons->addRow(tr("&Bottom (x)"), text_botx);

    buttons->setFormAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    CameraImageRenderer *image = new CameraImageRenderer(this->handler);
    image->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    nlayout->addWidget(image, 3);
    nlayout->addLayout(buttons);

    this->setLayout(nlayout);
    this->canvas = image;

    QObject::connect(text_topx, &QLineEdit::textChanged, this, &CameraCalibratorWidget::redrawSignal);
    QObject::connect(text_topy, &QLineEdit::textChanged, this, &CameraCalibratorWidget::redrawSignal);
    QObject::connect(text_botx, &QLineEdit::textChanged, this, &CameraCalibratorWidget::redrawSignal);
}


void CameraCalibratorWidget::redraw() {
    auto w = this->botx->text().toDouble() - this->topx->text().toDouble();

    if (auto lockedHandler = this->handler.lock(); auto lockedSettings = lockedHandler.get()->get_mask().lock()) {
        auto[mask, _] = lockedSettings.get()->getMutableMask();
        mask.o_x = this->topx->text().toDouble();
        mask.o_y = this->topy->text().toDouble();

        mask.w = w;
        mask.scale = this->canvas->get_scaleX();
    }


    canvas->update();
}

void CameraCalibratorWidget::redrawSignal() {
    redraw();
}
