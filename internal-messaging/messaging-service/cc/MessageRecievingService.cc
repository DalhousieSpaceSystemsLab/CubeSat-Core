//
// Created by Spencer Axford on 2019-09-07.
//

#include "MessageReceivingService.h"
#include "PhoneBook.h"
#include "MessageBuilder.h"
#include <string>

//Constructor creates UnixDomainStreamSocketServer and sets
 MessageReceivingService::MessageReceivingService(unsigned int identifier):
    server_socket_(PhoneBook::IdentifierToProcessFilePath(identifier)) {}

 MessageReceivingService::MessageReceivingService(unsigned int identifier, unsigned int capacity):
    server_socket_(PhoneBook::IdentifierToProcessFilePath(identifier)) {}


void MessageReceivingService::SetIdentifier(unsigned int identifier){
    std::string path = PhoneBook::IdentifierToProcessFilePath(identifier);
    server_socket_ = UnixDomainStreamSocketServer(path);
}

int MessageReceivingService::StartListeningForClients(){
    server_socket_.StartListening();
    return 0;
}


int MessageReceivingService::ListenForMessage(Message *&message, unsigned int message_capacity){
    std::string flat_message;
    server_socket_.HandleConnection(flat_message, message_capacity);
    cout << "Handling message: " << flat_message << endl; 
    if(MessageBuilder::BuildMessageFromFlattened(message, flat_message, message_capacity) == 0){
        throw "ERROR determining message type";
        return 0;
    }
    return 1;
}

int MessageReceivingService::Reply(Message &message){
    if(message.GetCapacity() > 0){
        char msg[message.GetCapacity()];
        message.Flatten(msg);
        server_socket_.ReplyToCurrentClient(msg, message.GetCapacity());
        return 0;
    }
    throw "Message Capacity is not set correctly";
    return 1;
}