#ifndef LORIS_CAMERA_CAMERAMANAGER_H
#define LORIS_CAMERA_CAMERAMANAGER_H

#include <iostream>
#include <vector>
#include "Camera.h"

using namespace std;

class CameraManager {
public:
    CameraManager() {

    }

    vector<Camera> cameras() {
        return this->cameras_;
    }

    void addCamera(Camera camera) {
        this->cameras_.push_back(camera);
    }

    vector<Camera> cameras_;
};

#endif
