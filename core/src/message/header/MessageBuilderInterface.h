
#ifndef LORIS_MESSAGE_MESSAGEBUILDERINTERFACE_H_
#define LORIS_MESSAGE_MESSAGEBUILDERINTERFACE_H_

#include "Message.h"
#include "KeyValuePairContainer.h"

class MessageBuilderInterface {
public:
    virtual void StartMessage() = 0;
    virtual Message CompleteMessage() = 0;

    virtual void SetRecipient(unsigned int recipient) = 0;
    virtual void SetSender(unsigned int sender) = 0;
    virtual void SetMessageContents(KeyValuePairContainer container) = 0;
};

#endif
