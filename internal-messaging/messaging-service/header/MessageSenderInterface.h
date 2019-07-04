//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_MESSAGESENDERINTERFACE_H
#define LORIS_MESSAGESENDERINTERFACE_H

#include "MessagingService.h"
#include "Message.h"

class MessageSenderInterface : protected MessagingService {

public:
    MessageSenderInterface(unsigned int recipient);
    void SetRecipient( unsigned int recipient);
    void SendMessage(Message message);
    void SendFlattenedMessage(char message[]);
};


#endif //LORIS_MESSAGESENDERINTERFACE_H
