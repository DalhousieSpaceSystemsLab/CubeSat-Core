#ifndef LORIS_CAMERA_CAMERAMANAGER_H
#define LORIS_CAMERA_CAMERAMANAGER_H

#include <iostream>
#include <vector>
#include "Camera.h"

using namespace std;

class CameraManager {
public:
    CameraManager();

    vector<Camera> cameras();

    void addCamera(Camera camera);

private:
    vector<Camera> cameras_;
};

#endif
