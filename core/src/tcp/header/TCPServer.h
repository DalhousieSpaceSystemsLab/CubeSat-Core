#ifndef LORIS_TCP_TCP_SERVER_H_
#define LORIS_TCP_TCP_SERVER_H_

#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

/* 
	A simple server in the internet domain using TCP
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class TCPServer
{

  private:
    int socket_file_descriptor_; //Socket file descriptor
    int port_number_;            //Server port number
    socklen_t client_address_size;
    int new_socket_file_descriptor_;
    int socket_io_status_;
    char buffer_[256];
    struct sockaddr_in server_address_;
    struct sockaddr_in client_address_;

    void BindSocketToIpAddress(int socket_file_descriptor, struct sockaddr_in server_address);

  public:
    TCPServer(int port_number);

    //TODO Add a virtual function that allows the server to perform some operation in between waiting
    //TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses. 
    //TODO Checkout "fcntl". May potentially allow non-blocking mode
    void WaitForConnection();

    void ResetBuffer();

    void ToString();

    
    int ReceiveRequest(char *buffer, int new_socket_file_descriptor);

    //Return 0 if request handled successfully
    //Return 1 if request handling failed
    virtual int HandleRequest(char *buffer,int new_socket_file_descriptor);

    void WriteToClient(const char * msg,int new_socket_file_descriptor);

    void error(const char *msg);
};

#endif // LORIS_TCP_TCP_SERVER_H_