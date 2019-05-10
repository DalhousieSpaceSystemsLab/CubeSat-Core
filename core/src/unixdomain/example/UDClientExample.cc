//
// Created by Spencer Axford on 2019-05-07.
//

#include "UDClientExample.h"

UDClientExample::UDClientExample(char *sun_path)
        : UnixDomainStreamSocketClient(sun_path) {}

int UDClientExample::HandleMessage(char *buffer,int new_socket_file_descriptor){
    cout << "Handling message " << buffer << endl;
    return 0;
}

int UDClientExample::Send(char message[]) {
    if (WriteToSocket(message, socket_file_descriptor_) != 0) {
        error("ERROR SENDING MESSAGE");
        return 1;
    }
    return 0;
}

int UDClientExample::SendMessageAwaitReply(char message[]) {
    if (WriteToSocket(message, socket_file_descriptor_) != 0) {
        error("ERROR SENDING MESSAGE");
        return 1;
    }
    if (ReadFromSocket(socket_file_descriptor_) != 0) {
        error("ERROR READING FROM SOCKET");
        return 1;
    }
    return 0;
}