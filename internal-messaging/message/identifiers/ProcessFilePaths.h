//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef DALCUBESAT_PROCESSFILEPATHS_H
#define DALCUBESAT_PROCESSFILEPATHS_H

#include <string>

struct ProcessFilePaths {
    const std::string power_repository = "/tmp/ipc/repositories/power.sock";
    const std::string comms_repository = "/tmp/ipc/repositories/comms.sock";
    const std::string gps_repository = "/tmp/ipc/repositories/adcs.sock";
    const std::string payload_repository = "/tmp/ipc/repositories/payload.sock";
    const std::string adcs_repository = "/tmp/ipc/repositories/gps.sock";
};

#endif //DALCUBESAT_PROCESSFILEPATHS_H