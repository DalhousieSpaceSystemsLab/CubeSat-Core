//  HardwareEmulationManager.h
//      Basic implementation for the HardwareEmulationManager class
//


#ifndef CUBESAT_HARDWAREEMULATIONSERVER_H
#define CUBESAT_HARDWAREEMULATIONSERVER_H

#include <unistd.h>
#include "SubsystemEmulator.h"
#include "Message.h"
#include <vector>

class HardwareEmulationManager {
public:
    HardwareEmulationManager();
//TODO implement this constructor

 //   HardwareEmulationManager(std::vector<SubsystemEmulator *> emulators);

    void Run();

    void UpdateEmulators();
    
    void SendDataToCore(Message data);


private:
    std::vector<SubsystemEmulator *> emulators_;
    long time_between_updates_;
    long current_time_;
};

#endif
