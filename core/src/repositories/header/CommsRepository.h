//
// Created by John Barry on 2019-05-16.
//

#ifndef LORIS_COMMSREPOSITORY_H
#define LORIS_COMMSREPOSITORY_H

#include "Repository.h"
// #include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class CommsRepository : public Repository {
public:
    CommsRepository(std::string filePaths);
    int ProcessMessage(DataMessage message);

    int AddKeysToWatchList();

};


#endif //LORIS_COMMSREPOSITORY_H
