//
// Created by Dawid Kulikowski on 17/10/2021.
//

#ifndef CHESSTASTIC_CAMERADATASETGENERATOR_H
#define CHESSTASTIC_CAMERADATASETGENERATOR_H

#include <QWidget>
#include "CameraHandler.h"
#include <QLineEdit>

class CameraDataSetGenerator : public QWidget {
Q_OBJECT
public:
    CameraDataSetGenerator(std::weak_ptr<CameraHandler>, QWidget *parent = nullptr);

protected slots:

    void save_image();

private:
    QLineEdit *name_field;
    QLineEdit *row_ignore_field;
    QLineEdit *column_ignore_field;

    std::weak_ptr<CameraHandler> handler;

    int cid = 0;
};


#endif //CHESSTASTIC_CAMERADATASETGENERATOR_H
