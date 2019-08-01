//
// Created by John Barry on 2019-05-16.
//

#ifndef LORIS_GPSREPOSITORY_H
#define LORIS_GPSREPOSITORY_H

#include "Repository.h"
// #include "../../message/identifiers/ProcessFilePaths.h"
#include <string>

class GpsRepository : public Repository {
public:
    GpsRepository(std::string filePaths);
    int ProcessMessage(DataMessage message);

};


#endif //LORIS_GPSREPOSITORY_H
