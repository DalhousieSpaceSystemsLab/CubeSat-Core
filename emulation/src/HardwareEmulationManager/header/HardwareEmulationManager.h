//  HardwareEmulationManager.h
//      Basic implementation for the HardwareEmulationManager class
//
#include "SubsystemEmulator.h"
#include <unistd.h>
#include <vector>

#ifndef CUBESAT_HARDWAREEMULATIONSERVER_H
#define CUBESAT_HARDWAREEMULATIONSERVER_H

class HardwareEmulationManager {
public:
    HardwareEmulationManager();

    HardwareEmulationManager(std::vector<SubsystemEmulator> emulators);

    void Run();

    void UpdateEmulators();
    
    void SendDataToCore(Message data);


private:
    std::vector<SubsystemEmulator> emulators_;
    long time_between_updates_;
    long current_time_;
};

#endif
