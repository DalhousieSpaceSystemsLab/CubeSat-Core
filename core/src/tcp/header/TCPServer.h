#ifndef LORIS_TCP_TCP_SERVER_H_
#define LORIS_TCP_TCP_SERVER_H_

#include <string>
#include <stdio.h>
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

    void BindSocketToIpAddress(int socket_file_descriptor, struct sockaddr_in server_address)
    {
        //Bind the socket to an IP address and port
        if (bind(socket_file_descriptor, (struct sockaddr *)(&server_address), sizeof(server_address)) < 0)
        {
            cout << "server_address_ size: " << sizeof(server_address) << endl;
            cout << "Error code: " << errno << endl;
            error("ERROR on binding");
        }
    }

  public:
    TCPServer(int port_number)
    {
        //TODO(andrewway) Clean up constructor. Break up into functions
        port_number_ = port_number;

        //Initialize the server_address_ struct
        server_address_.sin_family = AF_INET;
        server_address_.sin_port = htons(port_number_);
        server_address_.sin_addr.s_addr = INADDR_ANY;

        //AF_UNIX is to be used for communication between sockets on the same host system
        socket_file_descriptor_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_file_descriptor_ < 0)
        {
            error("ERROR opening socket");
        }

        int enable = 1;
        if (setsockopt(socket_file_descriptor_, SOL_SOCKET, SO_REUSEADDR,
                       &enable, sizeof(int)) < 0)
        {
            error("setsockopt(SO_REUSEADDR) failed");
        }

        ResetBuffer();

        ToString();

        BindSocketToIpAddress(socket_file_descriptor_, server_address_);

        cout << "Listening for connection on port " << port_number_ << "..." << endl;
        //Indicate that the socket is for listening
        listen(socket_file_descriptor_, 5);

        WaitForConnection();

    }

    //TODO Add a virtual function that allows the server to perform some operation in between waiting
    //TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses. 
    //TODO Checkout "fcntl". May potentially allow non-blocking mode
    void WaitForConnection(){
        while (true)
        {
            cout << "Waiting for connection..." << endl;
            //Wait for connection
            client_address_size = sizeof(client_address_);
            new_socket_file_descriptor_ = accept(socket_file_descriptor_,
                                                 (struct sockaddr *)&client_address_, &client_address_size);
            //TODO Avoid blocking? We need to keep looping sometimes when there is no client waiting. 
            //accept() blocks until connection is made
            if (new_socket_file_descriptor_ < 0)
                error("ERROR on accept");

            //Code only proceeds beyond this point if connection was made
            cout << "Handling new request" << endl;
            HandleRequest(buffer_, new_socket_file_descriptor_);
        }
    }

    void ResetBuffer()
    {
        //Set buffer to zeros
        bzero((char *)&server_address_, sizeof(server_address_));
    }

    void ToString()
    {
        //TODO(andrewway) Change to return string
        cout << "socket_file_descriptor_: " << socket_file_descriptor_ << endl;
        cout << "AF_INET: " << server_address_.sin_family << endl;
        cout << "sin_port: " << server_address_.sin_port << endl;
        cout << "server_address_.sin.s: " << server_address_.sin_addr.s_addr << endl;
        cout << "&server_address_: " << &server_address_ << endl;
        cout << "server_address_ size: " << sizeof(server_address_) << endl;
        cout << std::to_string(port_number_) << endl;
    }
    int HandleRequest(char *buffer, int new_socket_file_descriptor)
    {
        bzero(buffer, 256);

        int socket_io_status = read(new_socket_file_descriptor, buffer, 255);
        if (socket_io_status < 0)
            error("ERROR reading from socket");
        printf("Here is the message: %s", buffer);

        socket_io_status = write(new_socket_file_descriptor, "I got your message", 18);
        if (socket_io_status < 0)
            error("ERROR writing to socket");

        return 0;
    }

    void error(const char *msg)
    {
        perror(msg);
        exit(1);
    }
};

#endif // LORIS_TCP_TCP_SERVER_H_