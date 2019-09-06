//
// Created by Spencer Axford on 2019-05-15.
//

#include "PhoneBook.h"
#include <string>
#include <iostream>

std::string PhoneBook::IdentifierToProcessFilePath(unsigned int identifier){
    Identifiers ids;
    ProcessFilePaths filePaths;
    std::string filepath;
    switch(identifier){
        case ids.power_subsystem :
            //ipc/i2c
            break;
        case ids.comms_subsystem :
            //ipc/i2c
            break;
        case ids.adcs_subsystem :
            //ipc/i2c
            break;
        case ids.payload_subsystem :
            //ipc/i2c
            break;
        case ids.gps_subsystem :
            //ipc/i2c
            break;
        case ids.power_repository :
            return filePaths.power_repository;
        case ids.comms_repository :
            return filePaths.comms_repository;
        case ids.adcs_repository :
            return filePaths.adcs_repository;
        case ids.payload_repository :
         return filePaths.payload_repository;
        case ids.gps_repository :
            return filePaths.gps_repository;
        default:
            std::cout << "ERROR: Identifier" << identifier << " does not exist" << std::endl;
            break;
    }
    return "";
}
