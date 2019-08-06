//
// Created by John Barry on 2019-05-16.
//

#ifndef LORIS_ADCSREPOSITORY_H
#define LORIS_ADCSREPOSITORY_H

#include "Repository.h"
// #include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class AdcsRepository : public Repository {
public:
    AdcsRepository(std::string filePaths);
    int ProcessMessage(DataMessage message);
    int AddKeysToWatchList();

};


#endif //LORIS_ADCSAREPOSITORY_H
