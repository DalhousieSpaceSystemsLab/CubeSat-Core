
#ifndef LORIS_MESSAGE_MESSAGEHEADER_H_
#define LORIS_MESSAGE_MESSAGEHEADER_H_
#include "Message.h"

class MessageHeader {
public:
    MessageHeader(unsigned int recipient, unsigned int sender, long time_created);
    
    friend SerializeMessage(Message *message, char *data);
    
	friend Message DeserializeMessage(char *data);

private:
    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
};

#endif
