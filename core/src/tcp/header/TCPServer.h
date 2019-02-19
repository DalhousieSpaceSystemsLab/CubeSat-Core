/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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
//using namespace std;
using std::cout;
using std::endl;
using std::string;

class TCPServer
{

  private:
    int sockfd; //Socket file descriptor
    int portno; //Server port number
    socklen_t clilen;
    int newsockfd;
    int n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

  public:
    TCPServer(int portno_)
    {

        //TODO Clean up constructor. Break up into functions
        //AF_UNIX is to be used for communication between sockets on the same host system
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            error("ERROR opening socket");
        }

        int enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            error("setsockopt(SO_REUSEADDR) failed");

        //Set buffer to zeros
        bzero((char *)&serv_addr, sizeof(serv_addr));

        portno = portno_;

        //log("Port number: "+std::to_string(portno));
        cout << std::to_string(portno) << endl;

        //Initialize the serv_addr struct
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        cout << "sockfd: " << sockfd << endl;
        cout << "AF_INET: " << serv_addr.sin_family << endl;
        cout << "sin_port: " << serv_addr.sin_port << endl;
        cout << "serv_addr.sin.s: " << serv_addr.sin_addr.s_addr << endl;
        cout << "&serv_addr: " << &serv_addr << endl;
        cout << "serv_addr size: " << sizeof(serv_addr) << endl;

        //Bind the socket to an IP address and port
        if (bind(sockfd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr)) < 0)
        {
            cout << "serv_addr size: " << sizeof(serv_addr) << endl;
            cout << "Error code: " << errno << endl;
            error("ERROR on binding");
        }

        cout << "Listening for connection..." << endl;
        //Indicate that the socket is for listening
        listen(sockfd, 5);

        while (true)
        {
            //Wait for connection
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            //accept() blocks until connection is made
            if (newsockfd < 0)
                error("ERROR on accept");

            //Code only proceeds beyond this point if connection was made
            cout << "Handling new request" << endl;
            handle_request(buffer, newsockfd);
        }
    }
    int handle_request(char *buffer, int newsockfd)
    {
        bzero(buffer, 256);
        for (int i = 1; i < 3; i++)
        {
            cout << i << ": Waiting for message..." << endl;
            int n = read(newsockfd, buffer, 255);
            if (n < 0)
                error("ERROR reading from socket");
            printf("Here is the message: %s", buffer);

            n = write(newsockfd, "I got your message", 18);
            if (n < 0)
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
