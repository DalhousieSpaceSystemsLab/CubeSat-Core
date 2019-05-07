#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

#include <iostream>
#include "UnixDomainStreamSocket.h"

/*
	A simple client in the internet domain using Unix Domain Socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
class UnixDomainStreamSocketClient : protected UnixDomainStreamSocket {
public:
    UnixDomainStreamSocketClient(char sun_path[]);

    virtual int Send(char message[]) = 0;

    virtual int SendMessageAwaitReply(char message[]) = 0;


private:

    int ConnectToSocket(char sun_path[]);
};
