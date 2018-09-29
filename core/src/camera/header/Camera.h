#ifndef LORIS_CAMERA_CAMERA_H
#define LORIS_CAMERA_CAMERA_H

#include <iostream>
#include <vector>

using namespace std;

class Camera {
public:
    Camera();

    void takePicture();

    vector<vector<vector<int>>>  picture();

private:
    vector<vector<vector<int>>>  picture_;
};

#endif
