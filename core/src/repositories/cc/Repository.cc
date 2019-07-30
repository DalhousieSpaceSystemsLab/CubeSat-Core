//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}

int Repository::HandleMessage(char *buffer){
    cout << "Handling message: " << buffer << endl;
    DataMessage msg = DataMessage(buffer);
    ProcessMessage(msg);
    return 0;
}
