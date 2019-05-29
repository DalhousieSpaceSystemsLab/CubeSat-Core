//
// Created by Spencer Axford on 2019-05-04.
//

#ifndef DALCUBESAT_UNIXDOMAINSTREAMSOCKET_H
#define DALCUBESAT_UNIXDOMAINSTREAMSOCKET_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/un.h>

using std::cout;
using std::endl;
using std::string;

class UnixDomainStreamSocket {

private:

    char *path_;            //Server socket path
    char buffer_[256];
    int n_;

protected:

    int socket_file_descriptor_; //Socket file descriptor
    struct sockaddr_un socket_address_;


    int WriteToSocket(const char *msg, int new_socket_file_descriptor);

    int ReadFromSocket(int new_socket_file_descriptor);

    int InitializeSocket(char sun_path[]);

    void error(const char *msg);

    void ResetBuffer();

    void ClearAddress();

    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    virtual int HandleMessage(char *buffer) = 0;

};


#endif //DALCUBESAT_UNIXDOMAINSTREAMSOCKET_H
