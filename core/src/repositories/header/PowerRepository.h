//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef LORIS_POWERREPOSITORY_H
#define LORIS_POWERREPOSITORY_H

#include "Repository.h"
#include <string>

class PowerRepository : public Repository {
public:
    PowerRepository(unsigned int identifier);
    int ProcessMessage(DataMessage message);
    unsigned int repository_identifier();

    int AddKeysToWatchList();

};


#endif //LORIS_POWERREPOSITORY_H
