//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}
void Repository::HandleConnection(int file_descriptor){
    //ReadFromSocket(new_socket_file_descriptor_);
    cout << "hello! Here is the file_descriptor" << file_descriptor << endl;
     //WriteToSocket("Message Received", new_socket_file_descriptor_);
}
int Repository::HandleMessage(char *buffer){
    cout << "Handling message: " << buffer << endl;
    Message msg = Message(buffer);
    ProcessMessage(msg);
    return 0;
}
