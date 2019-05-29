//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef DALCUBESAT_POWERREPOSITORY_H
#define DALCUBESAT_POWERREPOSITORY_H

#include "Repository.h"
#include "../cc/Repository.cc"
#include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class PowerRepository : public Repository {
public:
    PowerRepository(std::string filePaths);
    int ProcessMessage(Message message);

};


#endif //DALCUBESAT_POWERREPOSITORY_H
