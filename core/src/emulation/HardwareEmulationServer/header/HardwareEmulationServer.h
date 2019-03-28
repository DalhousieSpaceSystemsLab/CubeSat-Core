//  HardwareEmulationServer.h/cc
//      Basic implementation for the HardwareEmulationServer class
//
#include "SubsystemEmulator.h"
#include <unistd.h>
#include <vector>

#ifndef CUBESAT_HARDWAREEMULATIONSERVER_H
#define CUBESAT_HARDWAREEMULATIONSERVER_H

class HardwareEmulationServer {
public:
    HardwareEmulationServer();

    HardwareEmulationServer(std::vector<SubsystemEmulator> emulators);

    void Run();

    void UpdateEmulators();

private:
    std::vector<SubsystemEmulator> emulators_;
    long time_between_updates_;
    long current_time_;
};

#endif
