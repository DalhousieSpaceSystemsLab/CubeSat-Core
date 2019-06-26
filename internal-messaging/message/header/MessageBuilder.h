
#ifndef LORIS_MESSAGE_MESSAGEBUILDER_H_
#define LORIS_MESSAGE_MESSAGEBUILDER_H_

#include "KeyValuePairContainer.h"
#include "Message.h"
#include <ctime>

class MessageBuilder {
public:
    MessageBuilder();

    void StartMessage();
    Message CompleteMessage();

    void SetRecipient(unsigned int recipient);
    void SetSender(unsigned int sender);
    void SetMessageContents(KeyValuePairContainer container);

private:
    unsigned int current_recipient_;
    unsigned int current_sender_;
    KeyValuePairContainer message_contents_;
};

#endif
