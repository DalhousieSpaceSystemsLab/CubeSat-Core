//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef LORIS_POWERREPOSITORY_H
#define LORIS_POWERREPOSITORY_H

#include "Repository.h"
// #include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class PowerRepository : public Repository {
public:
    PowerRepository(std::string filePaths);
    int ProcessMessage(DataMessage message);

};


#endif //LORIS_POWERREPOSITORY_H
