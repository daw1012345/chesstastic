//
// Created by Dawid Kulikowski on 13/10/2021.
//

#ifndef CHESSTASTIC_CAMERAFEEDBACK_H
#define CHESSTASTIC_CAMERAFEEDBACK_H

#include <opencv2/opencv.hpp>
#include "CameraDetectionSettings.h"
#include "CameraHandler.h"
#include <condition_variable>
#include "CameraLogger.h"

enum class CameraFeedbackState {
    IDLE, NO_MOTION, MOTION
};

class CameraFeedback {
public:
    const int AREA_MAX_THRESH = 10000;
    const int AREA_MIN_THRESH = 100;
    const int MIN_REQ_CONTOURS = 1;
    const int UPDATE_INTERVAL_STD = 1000;
    const int UPDATE_INTERVAL_ON_MOTION = 500;
    const int SKIP_FRAME_CNT = 5;
    const int FRAME_PULL_FPS = 5;

    CameraFeedback(std::weak_ptr<CameraHandler> handler) : data_source(handler) {}

    void run();

    std::vector<int> wait_for_move();

private:
    std::mutex frame_cache_mtx;
    std::vector<cv::Mat> frame_cache;

#ifdef DEBUG
    CameraLogger logger;
    int current_detection = 0;
#endif

    std::mutex mtx;
    std::condition_variable cv;
    bool move_requested;
    bool move_detected;
    std::vector<int> changed_idx;

    CameraFeedbackState state = CameraFeedbackState::IDLE;
    cv::Mat last_static_frame;

    std::weak_ptr<CameraHandler> data_source;
    std::thread frame_pull_thread;
    std::thread parse_thread;

    void run_loop();
    void frame_pull();

    bool check_for_motion(cv::Mat, cv::Mat);

    inline std::tuple<CameraFeedbackState, int> do_idle_iter();

    inline std::tuple<CameraFeedbackState, int> do_no_motion_iter();

    inline std::tuple<CameraFeedbackState, int> do_motion_iter();

    std::vector<int> run_identification_pipeline(cv::Mat, cv::Mat);

    void register_move(std::vector<int>);
};


#endif //CHESSTASTIC_CAMERAFEEDBACK_H
