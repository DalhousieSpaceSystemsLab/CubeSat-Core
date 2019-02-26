/* 
	A simple server in the internet domain using TCP
*/

//REF: http://www.linuxhowtos.org/C_C++/socket.htm
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

class TCPServer
{

  private:
    int socket_file_descriptor_; //Socket file descriptor
    int port_number_; //Server port number
    socklen_t clilen;
    int new_socket_file_descriptor_;
    int socket_io_status_;
    char buffer_[256];
    struct sockaddr_in server_address_;
    struct sockaddr_in client_address_;

  public:
    TCPServer(int port_number)
    {

        //TODO Clean up constructor. Break up into functions
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
        
		//Set buffer to zeros
        bzero((char *)&server_address_, sizeof(server_address_));

        port_number_ = port_number;

        //log("Port number: "+std::to_string(port_number_));
        cout << std::to_string(port_number_) << endl;

        //Initialize the server_address_ struct
        server_address_.sin_family = AF_INET;
        server_address_.sin_port = htons(port_number_);
        server_address_.sin_addr.s_addr = INADDR_ANY;

        cout << "socket_file_descriptor_: " << socket_file_descriptor_ << endl;
        cout << "AF_INET: " << server_address_.sin_family << endl;
        cout << "sin_port: " << server_address_.sin_port << endl;
        cout << "server_address_.sin.s: " << server_address_.sin_addr.s_addr << endl;
        cout << "&server_address_: " << &server_address_ << endl;
        cout << "server_address_ size: " << sizeof(server_address_) << endl;

        //Bind the socket to an IP address and port
        if (bind(socket_file_descriptor_, (struct sockaddr *)(&server_address_), sizeof(server_address_)) < 0)
        {
            cout << "server_address_ size: " << sizeof(server_address_) << endl;
            cout << "Error code: " << errno << endl;
            error("ERROR on binding");
        }

        cout << "Listening for connection on port "<< port_number_<< "..." << endl;
        //Indicate that the socket is for listening
        listen(socket_file_descriptor_, 5);

        while (true)
        {
            //Wait for connection
            clilen = sizeof(client_address_);
            new_socket_file_descriptor_ = accept(socket_file_descriptor_, 
                (struct sockaddr *)&client_address_, &clilen);
            //accept() blocks until connection is made
            if (new_socket_file_descriptor_ < 0)
                error("ERROR on accept");

            //Code only proceeds beyond this point if connection was made
            cout << "Handling new request" << endl;
            handle_request(buffer_, new_socket_file_descriptor_);
        }
    }
    int handle_request(char *buffer_, int new_socket_file_descriptor_)
    {
        bzero(buffer_, 256);
        for (int i = 1; i < 3; i++)
        {
            cout << i << ": Waiting for message..." << endl;
            socket_io_status_ = read(new_socket_file_descriptor_, buffer_, 255);
            if (socket_io_status_ < 0)
                error("ERROR reading from socket");
            printf("Here is the message: %s", buffer_);

            socket_io_status_ = write(new_socket_file_descriptor_, "I got your message", 18);
            if (socket_io_status_ < 0)
                error("ERROR writing to socket");
        }
        return 0;
    }

    void log(const string s)
    {
        char cstr[s.size() + 1];
        strcpy(cstr, s.c_str());
        log(cstr);
    }
    void log(const char *msg)
    {

        for (int i = 0; i < sizeof(msg); i++)
        {
            cout << msg[i];
        }
        cout << endl;
    }
    void error(const char *msg)
    {
        perror(msg);
        exit(1);
    }
};
