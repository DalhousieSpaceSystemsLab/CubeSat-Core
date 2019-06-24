//
// Created by Spencer Axford on 2019-05-07.
//

#include "UDServerExample.h"

UDServerExample::UDServerExample(std::string sun_path)
: UnixDomainStreamSocketServer(sun_path) {}

int UDServerExample::HandleMessage(char *buffer){
    cout << "Handling message " << buffer << endl;
    return 0;
}