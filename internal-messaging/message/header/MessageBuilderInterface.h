
#ifndef LORIS_MESSAGE_MESSAGEBUILDERINTERFACE_H_
#define LORIS_MESSAGE_MESSAGEBUILDERINTERFACE_H_

#include "DataMessage.h"
#include "KeyValuePairContainer.h"

//Interface for Message Builder 
class MessageBuilderInterface {
public:
    virtual void StartMessage() = 0;
    virtual DataMessage CompleteMessage() = 0;

    virtual void SetRecipient(unsigned int recipient) = 0;
    virtual void SetSender(unsigned int sender) = 0;
    virtual void SetMessageContents(KeyValuePairContainer container) = 0;
};

#endif
