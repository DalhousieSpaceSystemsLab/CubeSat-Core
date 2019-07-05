//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGINGSERVICE_H
#define LORIS_MESSAGINGSERVICE_H

#include "UnixDomainStreamSocketClient.h"
#include "Identifiers.h"
#include <string>

//Used by MessageSenderInterface as a method to interact with a UnixDomainStreamSocketClient without providing direct read write access
class MessagingService {
public: 
    //Constructor
    //recipient is a indentifier indicating what repository should receive the messages
    MessagingService(unsigned int recipient);
private:

protected:
    //Converts Identifier (as int) to the filepath of the Unix Domain Socket
    std::string IdentifierToProcessFilePath(unsigned int identifier);
    UnixDomainStreamSocketClient client_socket_;
};


#endif //LORIS_MESSAGINGSERVICE_H
