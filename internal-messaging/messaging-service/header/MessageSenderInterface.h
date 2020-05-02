//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGESENDERINTERFACE_H
#define LORIS_MESSAGESENDERINTERFACE_H

#include "DataMessage.h"

//A Messaging interface that allows for sending messages to a desiginated recipient 
class MessageSenderInterface {

public:
    //Methods to send Message objects
    virtual string SendDataMessage(DataMessage message) = 0;
    //Commented out because SendDataMessage in MessageSendingService automatically waits if the message contains requests
    // virtual string SendDataMessageAwaitReply(DataMessage message) = 0;
protected:
    //Constructor
    MessageSenderInterface();

};

#endif //LORIS_MESSAGESENDERINTERFACE_H
