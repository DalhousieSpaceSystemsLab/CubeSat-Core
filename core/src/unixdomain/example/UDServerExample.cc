//
// Created by Spencer Axford on 2019-05-07.
//

#include "UDServerExample.h"

UDServerExample::UDServerExample(char *sun_path)
: UnixDomainStreamSocketServer(sun_path) {}

int UDServerExample::HandleMessage(char *buffer,int new_socket_file_descriptor){
    cout << "Handling message " << buffer << endl;
    return 0;
}