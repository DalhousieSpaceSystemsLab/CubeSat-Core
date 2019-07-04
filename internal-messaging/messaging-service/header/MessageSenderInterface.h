//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGESENDERINTERFACE_H
#define LORIS_MESSAGESENDERINTERFACE_H

#include "MessagingService.h"
#include "Message.h"

//A Messaging Service that allows for sending messages to a desiginated recipient 
class MessageSenderInterface : protected MessagingService {

public:
    //Constructor
    //recipient is Identifier int that specifies which repository the messages should be directed to (See Identifier.h)
    MessageSenderInterface(unsigned int recipient);
    //Setter for Recipient after object construction
    void SetRecipient( unsigned int recipient);
    
    //Methods to send Message objects
    void SendMessage(Message message);
    
private:
    //Method to send flattened message as char array 
    //message - Char array representing a Message object no larger then 256 bytes (Must be output of flatten method of Message object)
    void SendFlattenedMessage(char message[]);
};

#endif //LORIS_MESSAGESENDERINTERFACE_H
