#ifndef LORIS_CAMERA_CAMERA_H
#define LORIS_CAMERA_CAMERA_H

#include <iostream>
#include <vector>

using namespace std;

class Camera {
public:
    Camera() {

    }

    void takePicture() {
        this->picture_ = new int[1][1][1];
    }

    int * picture() {
        return &this->picture_;
    }

    int picture_[][][3];
};

#endif
