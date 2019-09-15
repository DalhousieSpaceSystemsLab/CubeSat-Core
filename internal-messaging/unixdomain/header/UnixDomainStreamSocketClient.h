#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include "UnixDomainStreamSocket.h"

/*
	A simple client in the internet domain using Unix Domain Socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class UnixDomainStreamSocketClient : protected UnixDomainStreamSocket {
public:
    //Constructor 
    //sock_path - local file system path to unix domain socket as string
    UnixDomainStreamSocketClient(std::string sock_path);

    //Sends message without awaiting reply
    //message - what data will be sent (should be below 256 bytes)
    int Send(char message[]);

    //Sends message awaiting reply from repository
    //message - what data will be sent (should be below 256 bytes)
    int SendMessageAwaitReply(char message[], string & reply);

    string GetReply();

private:
    //initial connection to Unix Domain socket for data transfer
    //sun_path - local file system path to unix domain socket as character array
    int ConnectToSocket(char sun_path[]);
};
