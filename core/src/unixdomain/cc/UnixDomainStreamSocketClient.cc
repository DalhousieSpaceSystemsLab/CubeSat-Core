#include "../header/UnixDomainStreamSocketClient.h"
#include "../cc/UnixDomainStreamSocket.cc"

/*
	A simple client in the internet domain using TCP
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
UnixDomainStreamSocketClient::UnixDomainStreamSocketClient(char sun_path[]) {
    InitializeSocket(sun_path);
    ConnectToSocket(socket_address_.sun_path);
}

int UnixDomainStreamSocketClient::ConnectToSocket(char sun_path[]) {
    std::cout << "Attempting connection to socket " << this->socket_file_descriptor_ << std::endl;
    if (connect(socket_file_descriptor_, (struct sockaddr *) &socket_address_, sizeof(socket_address_)) < 0) {
        error("ERROR connecting");
        return 1;
    }
    return 0;
}
