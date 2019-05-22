//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef DALCUBESAT_REPOSITORY_H
#define DALCUBESAT_REPOSITORY_H

//#include "../../message/header/MessageSerializer.h"
//#include "../../message/header/Message.h"
#include "../../unixdomain/header/UnixDomainStreamSocketServer.h"
#include <string>

class Repository : public UnixDomainStreamSocketServer {
public:
    Repository(std::string socket_path);
    int HandleMessage(char *buffer, int new_socket_file_descriptor);

private:
//    int ProcessMessage(Message message);
};


#endif //DALCUBESAT_REPOSITORY_H
