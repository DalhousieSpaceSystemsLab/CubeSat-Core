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
    PayloadRepository(std::string filePaths);
    int ProcessMessage(DataMessage message,int client_file_descriptor);
    int AddKeysToWatchList();
};


#endif //LORIS_PAYLOADREPOSITORY_H
