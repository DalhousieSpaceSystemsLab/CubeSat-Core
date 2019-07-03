//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef DALCUBESAT_MESSAGESENDERINTERFACE_H
#define DALCUBESAT_MESSAGESENDERINTERFACE_H

#include "MessagingService.h"
#include "Message.h"

class MessageSenderInterface : protected MessagingService {

public:
    MessageSenderInterface(unsigned int recipient);
    void SetRecipient( unsigned int recipient);
    void SendMessage(Message message);
    void SendFlattenedMessage(char message[]);
};


#endif //DALCUBESAT_MESSAGESENDERINTERFACE_H
