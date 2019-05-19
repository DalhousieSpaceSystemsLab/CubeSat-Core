//
// Created by Spencer Axford on 2019-05-07.
//

#ifndef DALCUBESAT_UXCLIENTEXAMPLE_H
#define DALCUBESAT_UXCLIENTEXAMPLE_H

#include "../cc/UnixDomainStreamSocketClient.cc"


class UDClientExample : public UnixDomainStreamSocketClient {
public:
    UDClientExample(char sun_path[]);

    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    int HandleMessage(char *buffer,int new_socket_file_descriptor);
};

#endif //DALCUBESAT_UXCLIENTEXAMPLE_H
