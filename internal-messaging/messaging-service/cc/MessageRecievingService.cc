//
// Created by Spencer Axford on 2019-09-07.
//

#include "MessageReceivingService.h"
#include "PhoneBook.h"
#include "MessageBuilder.h"
#include <string>

 MessageReceivingService::MessageReceivingService(unsigned int identifier):
    server_socket_(PhoneBook::IdentifierToProcessFilePath(identifier)) {}

void MessageReceivingService::SetIdentifier(unsigned int identifier){
    std::string path = PhoneBook::IdentifierToProcessFilePath(identifier);
    server_socket_ = UnixDomainStreamSocketServer(path);
}

int MessageReceivingService::StartListeningForClients(){
    server_socket_.StartListening();
    return 0;
}


int MessageReceivingService::ListenForMessage(Message *&message){
    std::string flat_message;
    server_socket_.HandleConnection(flat_message);
    cout << "Handling message: " << flat_message << endl; 
    if(MessageBuilder::BuildMessageFromFlattened(message, flat_message) == 0){
        throw "ERROR determining message type";
        return 0;
    }
    return 1;
}

int MessageReceivingService::Reply(Message &message){
    //TODO Message length should be set by repo - to come after refactor
    char msg[255] = "";
	message.Flatten(msg);
    server_socket_.ReplyToCurrentClient(msg);
    return 0;
}