#include "HardwareEmulationServer.h"
#include "SubsystemEmulator.h"

HardwareEmulationServer::HardwareEmulationServer() {
    // null constructor
}

HardwareEmulationServer::HardwareEmulationServer(std::vector<SubsystemEmulator> emulators) {
    this->emulators_ = emulators;
}

void HardwareEmulationServer::Run() {
    while (true) {
        usleep(this->current_time_ * 1000);
        this->UpdateEmulators();
    }
}

void HardwareEmulationServer::UpdateEmulators() {
    for (int i=0; i < this->emulators_.size(); i++) {
        this->emulators_[i].Update(this->current_time_);
    }
}