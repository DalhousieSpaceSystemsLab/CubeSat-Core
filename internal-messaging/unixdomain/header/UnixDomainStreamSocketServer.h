#ifndef LORIS_UNIXDOMAIN_UNIXDOMAIN_SERVER_H_
#define LORIS_UNIXDOMAIN_UNIXDOMAIN_SERVER_H_

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

#include "UnixDomainStreamSocket.h"

using std::cout;
using std::endl;
using std::string;

/* 
	A simple server in the internet domain using Unix Domain Socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class UnixDomainStreamSocketServer : protected UnixDomainStreamSocket {

private:

    int new_socket_file_descriptor_;
    int socket_io_status_;
    int servlen, n;
    socklen_t client_address_size;
    struct sockaddr_un client_address_;

    //Initial set up binding server to socket 
    //socket_file_descriptor - current file descripter of socket
    //sock_address - local file system location of socket
    void BindSocketToAddress(int socket_file_descriptor, struct sockaddr_un sock_address);

    //Cleanly prints server information
    void ToString();

public:
    //Constructor
    //sock_path - local file system path to unix domain socket as string
    UnixDomainStreamSocketServer(string sock_path);


    //TODO Add a virtual function that allows the server to perform some operation in between waiting
    //TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses.
    //TODO Checkout "fcntl". May potentially allow non-blocking mode
    //Method to poll socket for new connections from UnixDomainStreamSocketClients
    //Calls ReadFromSocket once a connection is found
    virtual void WaitForConnection();
};

#endif // LORIS_UNIXDOMAIN_UNIXDOMAIN_SERVER_H_
