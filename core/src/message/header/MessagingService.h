//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef DALCUBESAT_MESSAGINGSERVICE_H
#define DALCUBESAT_MESSAGINGSERVICE_H

#include "../identifiers/Identifiers.h"
#include "../../unixdomain/cc/UnixDomainStreamSocketClient.cc"

#include <string>

class MessagingService {
public: 
    MessagingService(unsigned int recipient);
private:
    Identifier identifier_;
protected:
    std::string IdentifierToProcessFilePath(unsigned int identifier);
    UnixDomainStreamSocketClient client_socket_;
};


#endif //DALCUBESAT_MESSAGINGSERVICE_H
