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
        this->picture_ = vector<vector<vector<int>>>(1,vector<vector<int>>(1,vector <int>(3)));;
    }

    vector<vector<vector<int>>>  picture() {
        return this->picture_;
    }

    vector<vector<vector<int>>>  picture_;
};

#endif
