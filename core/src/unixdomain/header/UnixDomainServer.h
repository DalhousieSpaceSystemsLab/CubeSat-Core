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

#include "InterProcessCommunicationInterface.h"


using std::cout;
using std::endl;
using std::string;

/* 
	A simple server in the internet domain using Unix Domain Socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class UnixDomainServer : public InterProcessCommunicationInterface
{

  private:
    int socket_file_descriptor_; //Socket file descriptor
    int new_socket_file_descriptor_;
    int socket_io_status_;
    int servlen, n;
    socklen_t client_address_size;
    struct sockaddr_un  client_address_, server_address_;
    char buffer_[256];
    char *path_;            //Server socket path

    void BindSocketToSockAddress(int socket_file_descriptor, struct sockaddr_un sock_address);

    //TODO Add a virtual function that allows the server to perform some operation in between waiting
    //TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses.
    //TODO Checkout "fcntl". May potentially allow non-blocking mode
    void WaitForConnection();

    void ResetBuffer();

    void ToString();

    int ReceiveRequest(char *buffer, int new_socket_file_descriptor);

    void WriteToClient(const char * msg,int new_socket_file_descriptor);

    void error(const char *msg);

  public:
    UnixDomainServer(char sun_path[]);

    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    virtual int HandleRequest(char *buffer,int new_socket_file_descriptor) = 0;

};

#endif // LORIS_UNIXDOMAIN_UNIXDOMAIN_SERVER_H_