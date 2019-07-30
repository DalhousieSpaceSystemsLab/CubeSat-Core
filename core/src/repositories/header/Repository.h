//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef LORIS_REPOSITORY_H
#define LORIS_REPOSITORY_H

#include "DataMessage.h"
#include "UnixDomainStreamSocketServer.h"
#include <string>

class Repository : public UnixDomainStreamSocketServer {
public:
    Repository(std::string socket_path);
    int HandleMessage(char *buffer);

private:
   void HandleConnection(int file_descriptor);
   virtual int ProcessMessage(DataMessage message) = 0;
};


#endif //LORIS_REPOSITORY_H
