//
// Created by Dawid Kulikowski on 13/10/2021.
//

#include <thread>
#include <QApplication>
#include "CameraFeedback.h"
#include <chrono>
#include <ChessBoardRenderer.h>

void CameraFeedback::run() {
    if (this->parse_thread.joinable()) return;
    if (this->frame_pull_thread.joinable()) return;

    std::thread pull_thread(&CameraFeedback::frame_pull, this);
    std::thread main_thread(&CameraFeedback::run_loop, this);

    if (auto frame_src = this->data_source.lock()) {
        this->last_static_frame = frame_src->next_frame();
    }

    std::scoped_lock lck(mtx);

    this->move_requested = false;
    this->move_detected = false;

    this->parse_thread = std::move(main_thread);
    this->frame_pull_thread = std::move(pull_thread);
}

std::tuple<CameraFeedbackState, int> CameraFeedback::do_motion_iter() {
    if (auto frame_src = this->data_source.lock()) {
        std::scoped_lock frame_cache_lck(this->frame_cache_mtx);
        int start_idx;

        if (frame_cache.size() >= SKIP_FRAME_CNT + 2) {
            start_idx = SKIP_FRAME_CNT;
        } else if (frame_cache.size() > 2) {
            start_idx = 0;
        } else {
            return std::make_tuple(CameraFeedbackState::MOTION, UPDATE_INTERVAL_ON_MOTION);
        }

        auto curr = frame_cache.at(start_idx);
        auto next = frame_cache.at(start_idx + 1);

        if (!this->check_for_motion(frame_src->mask(curr), frame_src->mask(next))) {
//            Taking into consideration out application, we don't have to walk back to get the correct changes
//            do { // Walk back until we find the first frame with changes
//                start_idx--;
//
//                curr = frame_cache.at(start_idx);
//                next = frame_cache.at(start_idx + 1);
//            } while (!this->check_for_motion(frame_src->mask(curr), frame_src->mask(next)) && start_idx + 1 > frame_cache.size() - 1);
            auto changes = this->run_identification_pipeline(this->last_static_frame, next);
            this->last_static_frame = next;

            this->frame_cache.clear();

            this->register_move(changes);
            return std::make_tuple(CameraFeedbackState::IDLE, UPDATE_INTERVAL_STD);

        } else {
            this->frame_cache.erase(frame_cache.begin(), frame_cache.begin() + start_idx);
        }
    }

    return std::make_tuple(CameraFeedbackState::MOTION, UPDATE_INTERVAL_ON_MOTION);
}

std::tuple<CameraFeedbackState, int> CameraFeedback::do_no_motion_iter() {
    if (auto frame_src = this->data_source.lock()) {
        std::scoped_lock cache_lck(this->frame_cache_mtx);

        int frame_idx;

        if (this->frame_cache.size() > SKIP_FRAME_CNT) {
            frame_idx = SKIP_FRAME_CNT;
        } else if (!this->frame_cache.empty()) {
            frame_idx = this->frame_cache.size() - 1;
        } else {
            return std::make_tuple(CameraFeedbackState::NO_MOTION, UPDATE_INTERVAL_STD);
        }

        auto curr = this->last_static_frame;
        auto next = this->frame_cache.at(frame_idx);
        if (this->check_for_motion(frame_src->mask(curr), frame_src->mask(next))) {
            std::cout << "[FEEDBACK] Motion!" << std::endl;
            return std::make_tuple(CameraFeedbackState::MOTION, UPDATE_INTERVAL_ON_MOTION);
        } else {
            this->frame_cache.erase(frame_cache.begin(), frame_cache.begin() + frame_idx);
        }
    }

    return std::make_tuple(CameraFeedbackState::NO_MOTION, UPDATE_INTERVAL_STD);
}

std::tuple<CameraFeedbackState, int> CameraFeedback::do_idle_iter() {
    if (auto frame_src = this->data_source.lock()) {
        frame_src->next_frame();
    }

    std::scoped_lock lck(this->mtx);
    if (this->move_requested) {
        return {CameraFeedbackState::NO_MOTION, 0};
    }

    // Ignore unused frames
    std::scoped_lock lck_cache(this->frame_cache_mtx);
    this->frame_cache.clear();

    return {CameraFeedbackState::IDLE, UPDATE_INTERVAL_STD};
}

void  CameraFeedback::frame_pull() {
    while(!QApplication::closingDown()) {
        {
            std::scoped_lock lck(this->frame_cache_mtx);
            if (auto frame_src = this->data_source.lock()) {
                auto frame = frame_src->next_frame();
                this->frame_cache.push_back(frame);
            }
        }

        int delay = 1000/FRAME_PULL_FPS;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void CameraFeedback::run_loop() {
    auto NEXT_IVAL = UPDATE_INTERVAL_STD;
    while (!QApplication::closingDown()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(NEXT_IVAL));

        switch (this->state) {
            case CameraFeedbackState::NO_MOTION: {
                std::cout << "[FEEDBACK] NO MOTION TICK" << std::endl;
                auto[next, iter] = this->do_no_motion_iter();
                this->state = next;
                NEXT_IVAL = iter;
            }
                break;
            case CameraFeedbackState::MOTION: {
                std::cout << "[FEEDBACK] MOTION TICK" << std::endl;
                auto[next, iter] = this->do_motion_iter();
                this->state = next;
                NEXT_IVAL = iter;
            }
                break;
            case CameraFeedbackState::IDLE: {
                std::cout << "[FEEDBACK] IDLE TICK" << std::endl;
                auto[next, iter] = this->do_idle_iter();
                this->state = next;
                NEXT_IVAL = iter;
            }
                break;
        }
    }
}

std::vector<int> CameraFeedback::run_identification_pipeline(cv::Mat old, cv::Mat next) {
    std::vector<int> results;

    if (auto frame_src = this->data_source.lock()) {
        auto old_components = frame_src->chop_frame(old);
        auto new_components = frame_src->chop_frame(next);
        assert(old_components.size() == new_components.size());
        for (int i = 0; i < old_components.size(); i++) {
            if (check_for_motion(old_components.at(i), new_components.at(i))) {
                results.push_back(i);
                std::cout << "[FEEDBACK] Movement on: " << std::to_string(i) << std::endl;
            }
        }
    }

    return results;
}

bool CameraFeedback::check_for_motion(cv::Mat rhs, cv::Mat lhs) {
    cv::Mat prhs;
    cv::Mat plhs;

    if (rhs.empty() || lhs.empty()) return false;
    cv::cvtColor(rhs, prhs, cv::COLOR_BGR2GRAY);
    cv::cvtColor(lhs, plhs, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(prhs, prhs, {21, 21}, 0);
    cv::GaussianBlur(plhs, plhs, {21, 21}, 0);

    cv::Mat raw_diff;
    cv::absdiff(prhs, plhs, raw_diff);

    cv::threshold(raw_diff, raw_diff, 30, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(raw_diff, contours, cv::RETR_TREE, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);


    std::remove_if(contours.begin(), contours.end(), [&](auto elem) {
        auto area = cv::contourArea(elem);

        return area < AREA_MIN_THRESH || area > AREA_MAX_THRESH;
    });

    return contours.size() >= MIN_REQ_CONTOURS;
}

std::vector<int> CameraFeedback::wait_for_move() {
    std::unique_lock lck(this->mtx);
    this->move_detected = false;
    this->move_requested = true;

    cv.wait(lck, [this] { return move_detected; });

    return changed_idx;
}

void CameraFeedback::register_move(std::vector<int> d_move) {
    std::scoped_lock lck(this->mtx);

    this->changed_idx = d_move;
    this->move_detected = true;
    this->move_requested = false;
    this->cv.notify_all();

}


