//
// Created by Spencer Axford on 2019-05-07.
//
//DEPRECIATED
#ifndef LORIS_UDSERVEREXAMPLE_H
#define LORIS_UDSERVEREXAMPLE_H

#include "../header/UnixDomainStreamSocketServer.h"
#include <string>

//This is an example of implimenting a Unix Domain Stream Socket Server
//This was a primitive example of making what is now called a repository in /core
class UDServerExample : public UnixDomainStreamSocketServer {


public:

    UDServerExample(std::string sun_path);
    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    int HandleMessage(char *buffer);
};


#endif //LORIS_UDSERVEREXAMPLE_H
