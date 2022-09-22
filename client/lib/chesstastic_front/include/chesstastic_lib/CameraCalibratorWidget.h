//
// Created by Dawid Kulikowski on 13/10/2021.
//

#ifndef CHESSTASTIC_CAMERACALIBRATORWIDGET_H
#define CHESSTASTIC_CAMERACALIBRATORWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "CameraImageRenderer.h"
#include "CameraDetectionSettings.h"
#include "CameraHandler.h"

class CameraCalibratorWidget : public QWidget {
Q_OBJECT
public:
    CameraCalibratorWidget(std::weak_ptr<CameraHandler>, QWidget *parent = nullptr);

protected slots:

    void redrawSignal();

private:
    QLineEdit *topx;
    QLineEdit *botx;
    QLineEdit *topy;

    std::weak_ptr<CameraHandler> handler;

    CameraImageRenderer *canvas;

    void redraw();
};


#endif //CHESSTASTIC_CAMERACALIBRATORWIDGET_H
