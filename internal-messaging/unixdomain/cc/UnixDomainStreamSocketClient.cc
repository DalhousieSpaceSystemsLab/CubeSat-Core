#include "UnixDomainStreamSocketClient.h"

//
//	A simple client in the internet domain using TCP
//
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
UnixDomainStreamSocketClient::UnixDomainStreamSocketClient(std::string sock_path) {
    
    int n = sock_path.length();

    // declaring character array
    char sun_path[n + 1];

    // copying the contents of the
    // string to char array
    strcpy(sun_path, sock_path.c_str());

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

int UnixDomainStreamSocketClient::Send(char message[]) {
    if (WriteToSocket(message, socket_file_descriptor_) != 0) {
        error("ERROR SENDING MESSAGE");
        return 1;
    }
    return 0;
}

int UnixDomainStreamSocketClient::SendMessageAwaitReply(char message[]) {
    if (WriteToSocket(message, socket_file_descriptor_) != 0) {
        error("ERROR SENDING MESSAGE");
        return 1;
    }
    int capacity=255;
    if (ReadFromSocket(socket_file_descriptor_,capacity) != 0) {
        error("ERROR READING FROM SOCKET");
        return 1;
    }
    return 0;
}

//default implementation
int UnixDomainStreamSocketClient::HandleMessage(char *buffer){
    cout << "Handling message " << buffer << endl;
    return 0;
}
