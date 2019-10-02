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
#include <sys/time.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/un.h>

using std::cout;
using std::endl;
using std::string;

class UnixDomainStreamSocket {

private:

    char *path_;            //Server socket path
    int n_;

protected:

    int socket_file_descriptor_; //Socket file descriptor
    struct sockaddr_un socket_address_;

    //Writes message out to socket
    //new_socket_file_descripter - sets socket file descripter for the send action
    int WriteToSocket(const char *msg, int new_socket_file_descriptor, unsigned int buffer_capcity);

    //Reads message from socket with connection
    //new_socket_file_descripter - file descripter for socket file with waiting connection 
    //timeout - seconds before read operation times out, defaults to 60 seconds
    int ReadFromSocket(char* buffer, int new_socket_file_descriptor, int buffer_capacity, unsigned int timeout = 60);


    //Sets up initial connection to socket when first created using socket path
    //sun_path - path to unix domain socket
    int InitializeSocket(char sun_path[]);

    //called when error is found
    void error(const char *msg);

    //clears current socket address struct
    void ClearAddress();

};


#endif //LORIS_UNIXDOMAINSTREAMSOCKET_H
