#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <iostream>

/* 
	A simple client in the internet domain using TCP
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class TCPClient {
    public:
        TCPClient(int portno_);


    private:
        int sockfd_;
        int portno_;
        int n_;
        struct sockaddr_in serv_addr_;
        struct hostent *server_;
        char buffer_[256];

        void ConnectToServer();

        void SetHostServer();

        void OpenSocket();

        void WriteToServer();

        void ReadFromServer();

        void error(const char *msg);
};
