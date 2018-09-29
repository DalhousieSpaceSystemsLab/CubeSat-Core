#include "../header/CameraManager.h"

CameraManager::CameraManager() {

}

vector<Camera> CameraManager::cameras() {
    return this->cameras_;
}

void CameraManager::addCamera(Camera camera) {
    this->cameras_.push_back(camera);
}