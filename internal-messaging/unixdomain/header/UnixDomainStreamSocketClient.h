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
    //message - what data will be sent
    //message_capacity - must be equal to or larger then the number of characters to send
    int Send(char message[], unsigned int message_capacity);

    //Sends message awaiting reply from repository
    //message - what data will be sent (should have less or equal bytes to the message_capacity value)
    //reply - string that will contain the reply from the server
    //message_capacity - maximum number of characters that will be sent from message
    //reply_capacity - maximum number of characters that will be read from the server and added to reply
    int SendMessageAwaitReply(char message[], string & reply, unsigned int message_capacity, unsigned int reply_capacity);

private:
    //initial connection to Unix Domain socket for data transfer
    //sun_path - local file system path to unix domain socket as character array
    int ConnectToSocket(char sun_path[]);
};
