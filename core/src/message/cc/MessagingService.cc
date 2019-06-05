//
// Created by Spencer Axford on 2019-05-15.
//

#include "MessagingService.h"
#include "ProcessFilePaths.h"

#include <string>

MessagingService::MessagingService(unsigned int recipient) : 
    client_socket_(IdentifierToProcessFilePath(recipient)){}

std::string MessagingService::IdentifierToProcessFilePath(unsigned int identifier){
    identifier_ = static_cast<Identifier>(identifier);
    ProcessFilePaths filePaths;
    std::string filepath;
    switch(identifier_){
        case power_subsystem :
            //ipc/i2c
            break;
        case comms_subsystem :
            //ipc/i2c
            break;
        case adcs_subsystem :
            //ipc/i2c
            break;
        case payload_subsystem :
            //ipc/i2c
            break;
        case gps_subsystem :
            //ipc/i2c
            break;
        case power_repository :
            return filePaths.power_repository;
        case comms_repository :
            return filePaths.comms_repository;
        case adcs_repository :
            return filePaths.adcs_repository;
        case payload_repository :
         return filePaths.payload_repository;
        case gps_repository :
            return filePaths.gps_repository;
        default:
            std::cout << "ERROR Identifier does not exist";
            break;
    }
    return "";
}
