//
// Created by Spencer Axford on 2019-05-15.
//

#include "../header/MessageSenderInterface.h"
#include "MessagingService.cc"

MessageSenderInterface::MessageSenderInterface(unsigned int recipient):
    MessagingService(recipient) {}

void MessageSenderInterface::SetRecipient(unsigned int recipient){
    std::string path = IdentifierToProcessFilePath(recipient);
    client_socket_ = UnixDomainStreamSocketClient(path);
}


void MessageSenderInterface::SendMessage(char message[]) {
    client_socket_.Send(message);
}

