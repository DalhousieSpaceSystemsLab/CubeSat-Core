//  HardwareEmulationManager.h
//      Basic implementation for the HardwareEmulationManager class
//


#ifndef CUBESAT_HARDWAREEMULATIONSERVER_H
#define CUBESAT_HARDWAREEMULATIONSERVER_H

#include <unistd.h>
#include "SubsystemEmulator.h"
#include "DataMessage.h"
#include <vector>

class HardwareEmulationManager {
public:
    HardwareEmulationManager();
//TODO implement this constructor

 //   HardwareEmulationManager(std::vector<SubsystemEmulator *> emulators);

    long Run();

    void UpdateEmulators();
    
    void SendDataToCore(DataMessage data);

    void GetCurrentStateString(char * string, int capacity);

    void GetCurrentState(DataMessage & message);


private:
    std::vector<SubsystemEmulator *> emulators_;
    long true_time_between_updates_;
    long const emulation_time_increment_= 10000;//10s
    long current_time_;
};

#endif
