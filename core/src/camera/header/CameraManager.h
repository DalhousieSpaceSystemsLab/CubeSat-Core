#ifndef LORIS_CAMERA_CAMERAMANAGER_H
#define LORIS_CAMERA_CAMERAMANAGER_H

#include <iostream>

using namespace std;

class Camera {
public:
    CameraManager() {

    }

    std::vector<Camera> cameras() {
        return this->cameras_;
    }

    std::vector<Camera> cameras_;
};

#endif
