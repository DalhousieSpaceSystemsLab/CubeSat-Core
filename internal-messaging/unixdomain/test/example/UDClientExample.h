//
// Created by Spencer Axford on 2019-05-07.
//

#ifndef DALCUBESAT_UXCLIENTEXAMPLE_H
#define DALCUBESAT_UXCLIENTEXAMPLE_H

#include "../header/UnixDomainStreamSocketClient.h"

//This is an example of implimenting a Unix Domain Stream Socket Client
//Classes do not typically inherit from UnixDomainStreamSocketClient as Messager can be used to send Messages instead.
class UDClientExample : public UnixDomainStreamSocketClient {
public:
    UDClientExample(char sun_path[]);
};

#endif //DALCUBESAT_UXCLIENTEXAMPLE_H
