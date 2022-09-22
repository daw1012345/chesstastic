//
// Created by Dawid Kulikowski on 13/10/2021.
//

#ifndef CHESSTASTIC_CAMERAHANDLER_H
#define CHESSTASTIC_CAMERAHANDLER_H

#include <opencv2/opencv.hpp>
#include <thread>
#include "CameraDetectionSettings.h"


class CameraHandler {
public:
    CameraHandler(int camera_id = 0,
                  std::shared_ptr<CameraDetectionSettings> settings = std::make_shared<CameraDetectionSettings>());

    void begin();

    cv::Mat get_frame() const;

    bool check_mask(int, int, int, int, cv::Mat);

    cv::Mat next_frame();

    cv::Mat mask(cv::Mat old);

    std::weak_ptr<CameraDetectionSettings> get_mask();

    std::vector<cv::Mat> chop_frame(cv::Mat);

    double calculate_shift(int);

private:
    mutable std::mutex mtx;

    int camera_id;
    cv::VideoCapture capture;

    std::shared_ptr<CameraDetectionSettings> settings;

    cv::Mat current_img;
    std::vector<cv::Mat> squares;
};


#endif //CHESSTASTIC_CAMERAHANDLER_H
