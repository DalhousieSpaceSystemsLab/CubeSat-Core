//
// Created by Spencer Axford on 2019-09-07.
//

#include "MessageReceivingService.h"
#include "PhoneBook.h"

MessageReceivingService::MessageReceivingService(unsigned int identifier):
    server_socket_(PhoneBook::IdentifierToProcessFilePath(identifier)) {}

void MessageReceivingService::SetIdentifier(unsigned int identifier){
    std::string path = PhoneBook::IdentifierToProcessFilePath(identifier);
    server_socket_ = UnixDomainStreamSocketServer(path);
}

int ListenForMessage(Message &message){

}

int ReplyWithDataMessage(DataMessage message){

}