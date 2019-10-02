//
// Created by Spencer Axford on 2019-09-07.
//

#ifndef LORIS_MESSAGERECIPIENTINTERFACE_H
#define LORIS_MESSAGERECIPIENTINTERFACE_H

#include "Message.h"
#include "DataMessage.h"

//A Messaging interface that allows for recieving messages
class MessageRecipientInterface {

public:
    //Method to listen for, and recieve a message
    virtual int ListenForMessage(Message *&message, unsigned int message_capacity) = 0;
    //Method to reply to last connected sender
    virtual int Reply(Message &message) = 0;
protected:
    //Constructor
    MessageRecipientInterface();

};

#endif //LORIS_MESSAGERECIPIENTINTERFACE_H
