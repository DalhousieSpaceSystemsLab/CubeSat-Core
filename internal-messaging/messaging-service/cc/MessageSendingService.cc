//
// Created by Spencer Axford on 2019-05-15.
//

#include "MessageSendingService.h"
#include "PhoneBook.h"

MessageSendingService::MessageSendingService(unsigned int recipient):
    client_socket_(PhoneBook::IdentifierToProcessFilePath(recipient)) {}

void MessageSendingService::SetRecipient(unsigned int recipient){
    std::string path = PhoneBook::IdentifierToProcessFilePath(recipient);
    client_socket_ = UnixDomainStreamSocketClient(path);
}

//Send message to socket
void MessageSendingService::SendFlattenedMessage(char message[]) {
    client_socket_.Send(message);
}

//Send message to socket and await reply
int MessageSendingService::SendFlattenedMessageAwaitReply(char message[], string &reply) {
    client_socket_.SendMessageAwaitReply(message, reply);
    return 0;
}

string MessageSendingService::SendDataMessage(DataMessage message) {
    char msg[255] = "";
    string reply;
    message.Flatten(msg);
    if(message.HasRequests()){
    	SendFlattenedMessageAwaitReply(msg, reply);
        return reply;
    }
    else{
    	SendFlattenedMessage(msg);
    	return "";
    }
}
