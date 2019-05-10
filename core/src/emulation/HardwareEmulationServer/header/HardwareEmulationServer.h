//  HardwareEmulationServer.h/cc
//      Basic implementation for the HardwareEmulationServer class
//
#include "SubsystemEmulator.h"
#include "../PowerServer/header/PowerServer.h"
#include "../tcp/Serialize/Message.h"
#include "../tcp/Serialize/StructSerializer.h"
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
    
    void SendDataToServer(SubsystemStruct *data, int port_number, int new_socket_file_descriptor);

private:
    std::vector<SubsystemEmulator> emulators_;
    long time_between_updates_;
    long current_time_;
};

#endif
