//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGESENDINGSERVICE_H
#define LORIS_MESSAGESENDINGSERVICE_H

#include "DataMessage.h"
#include "UnixDomainStreamSocketClient.h"
#include "MessageSenderInterface.h"


//A Messaging Service that allows for sending messages to a desiginated recipient 
class MessageSendingService : protected MessageSenderInterface {

public:
    //Constructor
    //recipient is Identifier int that specifies which repository the messages should be directed to (See Identifier.h)
    MessageSendingService(unsigned int recipient);
    //Setter for Recipient after object construction
    void SetRecipient( unsigned int recipient);
    
    //Methods to send Message objects
    string SendDataMessage(DataMessage message) override;
    //Methods to send Message objects and specify a reply length
    string SendDataMessage(DataMessage message, unsigned int reply_capacity);
    //Commented out because SendDataMessage in MessageSendingService automatically waits if the message contains requests
    // string SendDataMessageAwaitReply(DataMessage message) override;
    
private:
    UnixDomainStreamSocketClient client_socket_;
    //Method to send flattened message as char array 
    //message - Char array representing a Message object no larger then message_size bytes (Must be output of flatten method of Message object)
    void SendFlattenedMessage(char message[], unsigned int messsage_size);

    //Method to send flattened message as char array and await reply from recipient
    //Message to be sent
    int SendFlattenedMessageAwaitReply(char message[], string &reply, unsigned int message_capacity, unsigned int reply_capcity);

};

#endif //LORIS_MESSAGESENDINGSERVICE_H