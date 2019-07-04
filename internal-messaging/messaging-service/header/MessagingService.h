//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGINGSERVICE_H
#define LORIS_MESSAGINGSERVICE_H

#include "UnixDomainStreamSocketClient.h"
#include "RecipientIdentifiers.h"
#include <string>

class MessagingService {
public: 
    MessagingService(unsigned int recipient);
private:

protected:
    std::string IdentifierToProcessFilePath(unsigned int identifier);
    UnixDomainStreamSocketClient client_socket_;
};


#endif //LORIS_MESSAGINGSERVICE_H
