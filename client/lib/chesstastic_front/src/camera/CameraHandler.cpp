//
// Created by Dawid Kulikowski on 13/10/2021.
//

#include "CameraHandler.h"
#include "ChessBoardRenderer.h"
#include <opencv2/opencv.hpp>


CameraHandler::CameraHandler(int camera_id, std::shared_ptr<CameraDetectionSettings> settings) : camera_id(camera_id),
                                                                                                 settings(settings) {}


void CameraHandler::begin() {
    std::scoped_lock lock(this->mtx);

    this->capture = cv::VideoCapture(this->camera_id);
}

cv::Mat CameraHandler::next_frame() {
    std::scoped_lock lock(this->mtx);

    cv::Mat out;
    this->capture >> out;

    cv::rotate(out, out, cv::ROTATE_90_COUNTERCLOCKWISE);
    cv::flip(out, out, 0);

    this->current_img = std::move(out);

    return this->current_img;
}

cv::Mat CameraHandler::get_frame() const {
    std::scoped_lock lock(this->mtx);

    return current_img;
}

std::weak_ptr<CameraDetectionSettings> CameraHandler::get_mask() {
    return this->settings;
}

cv::Mat CameraHandler::mask(cv::Mat old) {
    unsigned int mask_x;
    unsigned int mask_y;
    unsigned int mask_w;
    double scale;

    if (old.empty()) return old;

    {
        auto[mask, _] = this->settings.get()->getMutableMask();
        mask_x = mask.o_x;
        mask_y = mask.o_y;
        mask_w = mask.w;
        scale = mask.scale;
    }

    mask_x = mask_x / scale;
    mask_y = mask_y / scale;
    mask_w = mask_w / scale;

    if (check_mask(mask_x, mask_y, mask_w, scale, old)) return old;

    return old(cv::Range(mask_y, mask_y + mask_w), cv::Range(mask_x, mask_x + mask_w));

}

std::vector<cv::Mat> CameraHandler::chop_frame(cv::Mat frame) {
    std::vector<cv::Mat> result;

    unsigned int mask_x;
    unsigned int mask_y;
    unsigned int mask_w;
    double scale;

    if (frame.empty()) return {};

    {
        auto[mask, _] = this->settings.get()->getMutableMask();
        mask_x = mask.o_x;
        mask_y = mask.o_y;
        mask_w = mask.w;
        scale = mask.scale;
    }

    mask_x = mask_x / scale;
    mask_y = mask_y / scale;
    mask_w = mask_w / scale;

    if (check_mask(mask_x, mask_y, mask_w, scale, frame)) return {};

    const unsigned int INCREMENT = mask_w / ChessBoardRenderer::SIZE_OF_CHESS_BOARD;

    int y_idx = 0;
    for (unsigned int y = mask_y; y < mask_w + mask_y - INCREMENT; y += INCREMENT, y_idx++) {
        for (unsigned int x = mask_x; x < mask_w + mask_x - INCREMENT; x += INCREMENT) {
//            auto scaling = calculate_shift(y_idx);
            int lower_y = y;
            int higher_y = y + INCREMENT;

//            if (scaling > 1) {
//                higher_y *= scaling;
//            } else {
//                lower_y *= scaling;
//            }
            result.push_back(frame(cv::Range(lower_y, higher_y), cv::Range(x, x + INCREMENT)));
        }
    }

    return result;
}

bool CameraHandler::check_mask(int x, int y, int w, int s, cv::Mat matrix) {
    if (!w || !s) return false;
    if (y + w > matrix.rows || x + w > matrix.cols) return false;

    return true;
}

double CameraHandler::calculate_shift(int y) {

    if (y == 4) {
        return 1; // Middle of board
    }

    if (y < 4) {
        return 1 - 0.015 * 4/(y+1);
    }

    if (y > 4) {
        return 1 + 0.015* 4/(y+1);
    }

}


