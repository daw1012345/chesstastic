//
// Created by Dawid Kulikowski on 17/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICSETTINGSWINDOW_H
#define CHESSTASTIC_CHESSTASTICSETTINGSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include "CameraCalibratorWidget.h"
#include "CameraDataSetGenerator.h"

class ChesstasticSettingsWindow : public QWidget {
public:
    ChesstasticSettingsWindow(std::weak_ptr<CameraHandler> handlerPtr) : QWidget() {
        QVBoxLayout *layout = new QVBoxLayout;
        QTabWidget *main_tabs = new QTabWidget;
        QTabWidget *camera_tabs = new QTabWidget;

        CameraCalibratorWidget *calibrator = new CameraCalibratorWidget(handlerPtr);
        CameraDataSetGenerator *dataset = new CameraDataSetGenerator(handlerPtr);
        main_tabs->addTab(calibrator, "Calibration");
        main_tabs->addTab(dataset, "Training Assistant");

        layout->addWidget(main_tabs);
        this->setLayout(layout);
    }
};


#endif //CHESSTASTIC_CHESSTASTICSETTINGSWINDOW_H
