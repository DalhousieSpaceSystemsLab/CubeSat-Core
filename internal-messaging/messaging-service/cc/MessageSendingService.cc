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
void MessageSendingService::SendFlattenedMessage(char message[], unsigned int message_size) {
    client_socket_.Send(message, message_size);
}

//Send message to socket and await reply
int MessageSendingService::SendFlattenedMessageAwaitReply(char message[], string &reply, unsigned int message_capacity, unsigned int reply_capacity) {
    client_socket_.SendMessageAwaitReply(message, reply, message_capacity, reply_capacity);
    return 0;
}

string MessageSendingService::SendDataMessage(DataMessage message) {
    return SendDataMessage(message, DEFAULT_MESSAGE_CAPACITY);
}

string MessageSendingService::SendDataMessage(DataMessage message, unsigned int reply_capacity) {
    char msg[message.GetCapacity()];
    memset( msg, 0, message.GetCapacity()*sizeof(char) );
    message.Flatten(msg);
    cout << "sending message: " << msg << endl; 
    if(message.HasRequests()){
        string reply;
    	SendFlattenedMessageAwaitReply(msg, reply, message.GetCapacity(), reply_capacity);
        return reply;
    }
    else{
    	SendFlattenedMessage(msg, message.GetCapacity());
    	return "";
    }
}
