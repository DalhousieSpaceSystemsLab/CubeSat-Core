//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef DALCUBESAT_REPOSITORY_H
#define DALCUBESAT_REPOSITORY_H

#include "../../message/header/MessageHeader.h"
#include "../../unixdomain/header/UnixDomainStreamSocketServer.h"
#include <string>

class Repository : public UnixDomainStreamSocketServer {
public:
    Repository(std::string socket_path);
    int HandleMessage(char *buffer);

private:
   virtual int ProcessMessage(Message message) = 0;
};


#endif //DALCUBESAT_REPOSITORY_H
