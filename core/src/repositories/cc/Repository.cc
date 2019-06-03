//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include "../../unixdomain/cc/UnixDomainStreamSocketServer.cc"
#include "../../message/header/MessageSerializer.h"

#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}

int Repository::HandleMessage(char *buffer){
    cout << "Handling message: " << buffer << endl;
    Message msg = DeserializeMessage(buffer);
    ProcessMessage(msg);
    return 0;
}