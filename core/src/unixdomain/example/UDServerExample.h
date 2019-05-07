//
// Created by Spencer Axford on 2019-05-07.
//

#ifndef DALCUBESAT_UDSERVEREXAMPLE_H
#define DALCUBESAT_UDSERVEREXAMPLE_H

#include "../header/UnixDomainStreamSocketServer.h"
#include "../cc/UnixDomainStreamSocketServer.cc"


class UDServerExample : public UnixDomainStreamSocketServer {


public:

    UDServerExample(char sun_path[]);
    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    int HandleMessage(char *buffer,int new_socket_file_descriptor);
};


#endif //DALCUBESAT_UDSERVEREXAMPLE_H
