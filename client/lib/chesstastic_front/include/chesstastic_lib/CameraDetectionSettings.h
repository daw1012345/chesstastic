//
// Created by Dawid Kulikowski on 13/10/2021.
//

#ifndef CHESSTASTIC_CAMERADETECTIONSETTINGS_H
#define CHESSTASTIC_CAMERADETECTIONSETTINGS_H

#include <tuple>
#include <mutex>
#include <stdlib.h>

typedef struct {
    int o_x;
    int o_y;
    int w;
    double scale;
    std::string rotation;
} CameraMask;

typedef struct {
//    bool is_processing;
} DynamicProcessingVariables;

class CameraDetectionSettings {
public:
    auto getMutableMask() {
        return std::tuple<CameraMask &, std::scoped_lock<std::mutex>>{mask, this->mask_mtx};
    }

    auto getMutableProcessingVariables() {
        return std::tuple<DynamicProcessingVariables &, std::scoped_lock<std::mutex>>{pvars, this->pvars_mtx};
    }

private:
    mutable std::mutex mask_mtx;
    CameraMask mask;

    DynamicProcessingVariables pvars;
    mutable std::mutex pvars_mtx;
};


#endif //CHESSTASTIC_CAMERADETECTIONSETTINGS_H
