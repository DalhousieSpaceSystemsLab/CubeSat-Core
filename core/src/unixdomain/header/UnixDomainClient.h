#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

#include <iostream>

/* 
	A simple client in the internet domain using TCP
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class UnixDomainClient {
    public:
        UnixDomainClient(char sun_path[]);


    private:
        int sockfd_, servlen, n_;
        struct sockaddr_un serv_addr_;
        struct hostent *server_;
        char buffer_[256];

        void ConnectToServer(char sun_path[]);

        void SetHostServer();

        void OpenSocket();

        void WriteToServer();

        void ReadFromServer();

        void error(const char *msg);
};
