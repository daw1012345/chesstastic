//
// Created by Dawid Kulikowski on 08/11/2021.
//

#ifndef CHESSTASTIC_CAMERALOGGER_H
#define CHESSTASTIC_CAMERALOGGER_H

#include <string>
#include <opencv2/opencv.hpp>
#ifdef DEBUG
class CameraLogger {
public:
    CameraLogger(std::string path = "/tmp"): path(path) {};
    void save_frame(std::string, cv::Mat, std::string = "");
    void save_frame(int, cv::Mat, std::string = "");
private:
    std::string path;
    std::map<std::string, int> mapping;
};
#endif

#endif //CHESSTASTIC_CAMERALOGGER_H
