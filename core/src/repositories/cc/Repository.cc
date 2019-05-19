//
// Created by Spencer Axford on 2019-05-16.
//

#include "../header/Repository.h"
#include "../../unixdomain/cc/UnixDomainStreamSocketServer.cc"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}

//int Repository::ProcessMessage(Message message) {
//    return 0;
//}

int Repository::HandleMessage(char *buffer, int new_socket_file_descriptor){
    cout << "Handling message " << buffer << endl;
    return 0;
}