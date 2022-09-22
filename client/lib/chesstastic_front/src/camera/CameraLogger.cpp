//
// Created by Dawid Kulikowski on 08/11/2021.
//

#include "CameraLogger.h"
#ifdef DEBUG
void CameraLogger::save_frame(std::string folder, cv::Mat photo, std::string name) {
    int next = 0;

    if (this->mapping.count(folder) == 1) {
        next = this->mapping.at(folder) + 1;
    }

    this->mapping.insert_or_assign(folder, next);
    auto abs_path = this->path + "/" + folder + "_" + std::to_string(next) + "_" + name + ".png";
    std::cout << "[FEEDBACK] Logged to: " + abs_path << std::endl;
    cv::imwrite(abs_path, photo);
}

void CameraLogger::save_frame(int folder, cv::Mat img, std::string name) {
    return this->save_frame(std::to_string(folder), img, name);
}
#endif
