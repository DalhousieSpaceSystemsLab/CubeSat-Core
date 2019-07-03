//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}
void Repository::HandleConnection(int file_descriptor){
    cout << "Handling new connection..." << endl;
    int capacity=256;
    char buffer[capacity];
	ReadFromSocket(file_descriptor,buffer,capacity);
	cout << "Message from latest connection: " << endl;
    for(int i=0;i<sizeof(buffer);i++){
        cout << buffer[i];
    }
    cout << endl;
    //WriteToSocket("Message Received", new_socket_file_descriptor_);
}
int Repository::HandleMessage(char *buffer){
    cout << "Handling message: " << buffer << endl;
    Message msg = Message(buffer);
    ProcessMessage(msg);
    return 0;
}
