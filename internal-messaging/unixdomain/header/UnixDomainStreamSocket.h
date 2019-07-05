//
// Created by Spencer Axford on 2019-05-04.
//

#ifndef LORIS_UNIXDOMAINSTREAMSOCKET_H
#define LORIS_UNIXDOMAINSTREAMSOCKET_H

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

    //Writes message out to socket
    //new_socket_file_descripter - sets socket file descripter for the send action
    int WriteToSocket(const char *msg, int new_socket_file_descriptor);

    //Reads message from socket with connection
    //new_socket_file_descripter - file descripter for socket file with waiting connection 
    int ReadFromSocket(int new_socket_file_descriptor, int buffer_capacity);

    //Sets up initial connection to socket when first created using socket path
    //sun_path - path to unix domain socket
    int InitializeSocket(char sun_path[]);

    //called when error is found
    void error(const char *msg);

    //clears buffer
    void ResetBuffer();

    //clears current socket address struct
    void ClearAddress();
    
    //Gets called once a message has arrived to the socket, this should be implimented by any repository in order to process and create Message object
    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    virtual int HandleMessage(char *buffer) = 0;

};


#endif //LORIS_UNIXDOMAINSTREAMSOCKET_H
