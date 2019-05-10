
#ifndef LORIS_MESSAGE_MESSAGE_H_
#define LORIS_MESSAGE_MESSAGE_H_

#include "MessageHeader.h"
#include "KeyValuePairContainer.h"

class Message {
public:
    Message(MessageHeader header, KeyValuePairContainer contents);

private:
    MessageHeader header_;
    KeyValuePairContainer contents_;
};

#endif
