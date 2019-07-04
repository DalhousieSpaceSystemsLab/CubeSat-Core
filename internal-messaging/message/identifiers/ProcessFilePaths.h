//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_PROCESSFILEPATHS_H
#define LORIS_PROCESSFILEPATHS_H

#include <string>

//File paths to Unix Domain Sockets used for information transfer between processes
struct ProcessFilePaths {
    const std::string power_repository = "/tmp/ipc/repositories/power.sock";
    const std::string comms_repository = "/tmp/ipc/repositories/comms.sock";
    const std::string gps_repository = "/tmp/ipc/repositories/adcs.sock";
    const std::string payload_repository = "/tmp/ipc/repositories/payload.sock";
    const std::string adcs_repository = "/tmp/ipc/repositories/gps.sock";
};

#endif //LORIS_PROCESSFILEPATHS_H