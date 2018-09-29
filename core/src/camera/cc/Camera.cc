#include "../header/Camera.h"

Camera::Camera() {

}

void Camera::takePicture() {
    this->picture_ = vector<vector<vector<int>>>(1,vector<vector<int>>(1,vector <int>(3)));
}

vector<vector<vector<int>>> Camera::picture() {
    return this->picture_;
}
