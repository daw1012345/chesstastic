//
// Created by Dawid Kulikowski on 17/10/2021.
//

#include "CameraDataSetGenerator.h"
#include <QLineEdit>
#include <QPushButton>
#include <CameraHandler.h>
#include <QFormLayout>
#include <QIntValidator>
#include <QHBoxLayout>
#include <CameraImageRenderer.h>
#include <ChessBoardRenderer.h>

CameraDataSetGenerator::CameraDataSetGenerator(std::weak_ptr<CameraHandler> handlerPtr, QWidget *parent) : QWidget(
        parent), handler(handlerPtr) {
    QHBoxLayout *hlayout = new QHBoxLayout;
    QLineEdit *filename = new QLineEdit;
    this->name_field = filename;
    filename->setText("/tmp/dataset/prefix");
    QLineEdit *row_ignore = new QLineEdit;
    this->row_ignore_field = row_ignore;
    row_ignore->setValidator(new QIntValidator());
    QLineEdit *column_ignore = new QLineEdit;
    this->column_ignore_field = column_ignore;
    column_ignore->setValidator(new QIntValidator());

    QPushButton *capture = new QPushButton;
    capture->setText("Save");
    QObject::connect(capture, &QPushButton::pressed, this, &CameraDataSetGenerator::save_image);

    QFormLayout *form = new QFormLayout;
    form->setFormAlignment(Qt::AlignmentFlag::AlignVCenter);

    form->addRow(tr("&Path & Prefix"), filename);
    form->addRow(tr("&Ignore From Row"), row_ignore);
    form->addRow(tr("&Ignore From Column"), column_ignore);
    form->addRow(capture);
    hlayout->addWidget(new CameraImageRenderer(handlerPtr));
    hlayout->addLayout(form);
    this->setLayout(hlayout);
}

void CameraDataSetGenerator::save_image() {
    auto row_ignore = this->row_ignore_field->text().isEmpty() ? -1 : this->row_ignore_field->text().toInt();
    auto column_ignore = this->column_ignore_field->text().isEmpty() ? -1 : this->column_ignore_field->text().toInt();
    auto file_dst_prefix = this->name_field->text().toStdString();


    if (auto lockedHandler = this->handler.lock()) {
        auto components = lockedHandler.get()->chop_frame(lockedHandler->get_frame());
        for (int i = 0; i < components.size(); i++) {
            if (column_ignore > 0 && ((i % ChessBoardRenderer::SIZE_OF_CHESS_BOARD) >= column_ignore)) continue;
            if (row_ignore > 0 && (i / 8) >= row_ignore) continue;
            auto full_filename = file_dst_prefix + std::to_string(cid) + "_" + std::to_string(i) + ".png";
            cv::imwrite(full_filename, components.at(i));
        }
    }

    cid++;
}