//
// Created by Dawid Kulikowski on 14/10/2021.
//

#ifndef CHESSTASTIC_CAMERAIMAGERENDERER_H
#define CHESSTASTIC_CAMERAIMAGERENDERER_H

#include <QLabel>
#include <QPainter>
#include "CameraDetectionSettings.h"
#include "CameraHandler.h"
#include <QTimer>
#include <stdio.h>

class CameraImageRenderer : public QLabel {
Q_OBJECT
public:
    CameraImageRenderer(std::weak_ptr<CameraHandler> handler, QWidget *parent = nullptr) : QLabel("No data", parent),
                                                                                           handler(handler) {
        setScaledContents(false);
        this->setMinimumSize(500, 500);
        this->timer = new QTimer(this);

        connect(timer, &QTimer::timeout, this, QOverload<>::of(&CameraImageRenderer::timer_elapsed));
    }

    void setScaledPixmap(const QPixmap map) {
        if (map.isNull()) return;

        original_img = map;
        QLabel::setPixmap(scaled());
    };

    double get_scaleX() {
        double current_h = this->pixmap()->height();
        double original_h = this->original_img.height();
        return current_h / original_h;
    }

    double get_scaleY() {
        auto current_w = this->pixmap()->width();
        auto original_w = this->original_img.width();

        return current_w / original_w;
    }

private:
    QPixmap original_img;
    std::weak_ptr<CameraHandler> handler;

    static const int UPDATE_INTERVAL = 1000;

    QTimer *timer;

    QPixmap scaled() {
        return this->original_img.scaled(this->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::SmoothTransformation);
    }

protected slots:

    void resizeEvent(QResizeEvent *event) override {
        this->setScaledPixmap(this->original_img);
        QLabel::resizeEvent(event);
    }

    void showEvent(QShowEvent *ev) override {
        this->timer->start(UPDATE_INTERVAL);
        QWidget::showEvent(ev);
    }

    void hideEvent(QHideEvent *ev) override {
        this->timer->stop();
        QWidget::hideEvent(ev);
    }

    void timer_elapsed() {
        if (auto lockedHandler = this->handler.lock()) {
            auto frame = lockedHandler.get()->get_frame();

            if (frame.empty()) {
                return;
            }

            auto image_data = QPixmap::fromImage(
                    QImage(frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped());
            this->setScaledPixmap(std::move(image_data));

            std::cout << "[CALIBRATOR] Tick!" << std::endl;
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event);

        QPainter painter(this);

        if (!this->pixmap()) return;
        if (!this->original_img) return;
        auto pixmap_h = this->pixmap()->height();
        auto widget_h = this->height();
        auto offset_h = (widget_h - pixmap_h) / 2;

        auto pixmap_w = this->pixmap()->width();
        auto widget_w = this->width();
        auto offset_w = (widget_w - pixmap_w) / 2;

        painter.setPen(Qt::GlobalColor::red);

        if (auto lockedHandler = this->handler.lock(); auto lockedSettings = lockedHandler.get()->get_mask().lock()) {
            auto[mask, _] = lockedSettings.get()->getMutableMask();
            painter.drawRect( mask.o_x, mask.o_y, mask.w, mask.w);
        }
    }
};


#endif //CHESSTASTIC_CAMERAIMAGERENDERER_H
