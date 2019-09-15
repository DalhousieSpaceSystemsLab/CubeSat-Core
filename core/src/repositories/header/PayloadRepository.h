//
// Created by John Barry on 2019-05-16.
//

#ifndef LORIS_PAYLOADREPOSITORY_H
#define LORIS_PAYLOADREPOSITORY_H

#include "Repository.h"
// #include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class PayloadRepository : public Repository {
public:
    PayloadRepository(unsigned int identifier);
    int ProcessMessage(DataMessage message);
    int AddKeysToWatchList();

    unsigned int repository_identifier();
};


#endif //LORIS_PAYLOADREPOSITORY_H
