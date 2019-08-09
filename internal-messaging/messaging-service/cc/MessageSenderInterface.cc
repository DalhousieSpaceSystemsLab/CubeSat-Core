//
// Created by Spencer Axford on 2019-05-15.
//

#include "MessageSenderInterface.h"
//TODO this should not have implementation. Implementation should go in classes that implement this interface.
MessageSenderInterface::MessageSenderInterface(unsigned int recipient):
    MessagingService(recipient) {}

void MessageSenderInterface::SetRecipient(unsigned int recipient){
    std::string path = IdentifierToProcessFilePath(recipient);
    client_socket_ = UnixDomainStreamSocketClient(path);
}

void MessageSenderInterface::SendFlattenedMessage(char message[]) {
    client_socket_.Send(message);
}

void MessageSenderInterface::SendFlattenedMessageAndAwaitReply(char message[],string reply) {
    client_socket_.SendMessageAwaitReply(message);
    reply=client_socket_.GetBufferContents();
}


void MessageSenderInterface::SendDataMessage(DataMessage message) {
    char msg[255] = "";
    message.Flatten(msg);
    SendFlattenedMessage(msg);
}

void MessageSenderInterface::SendDataMessage(DataMessage message, string reply) {
    char msg[255] = "";

    message.Flatten(msg);

    SendFlattenedMessageAndAwaitReply(msg,reply);

}



