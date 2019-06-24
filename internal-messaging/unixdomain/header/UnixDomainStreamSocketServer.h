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

    void BindSocketToAddress(int socket_file_descriptor, struct sockaddr_un sock_address);

    void ToString();

public:
//    UnixDomainStreamSocketServer(char socket_path[]);
    UnixDomainStreamSocketServer(string sock_path);


    //TODO Add a virtual function that allows the server to perform some operation in between waiting
    //TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses.
    //TODO Checkout "fcntl". May potentially allow non-blocking mode
    virtual void WaitForConnection();
};

#endif // LORIS_UNIXDOMAIN_UNIXDOMAIN_SERVER_H_