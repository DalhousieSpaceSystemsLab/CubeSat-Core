
#ifndef LORIS_MESSAGE_MESSAGEBUILDER_H_
#define LORIS_MESSAGE_MESSAGEBUILDER_H_

#include "KeyValuePairContainer.h"
#include "DataMessage.h"
#include <ctime>

//Class used to build Message Objects
class MessageBuilder {
public:
    //Constructor
    MessageBuilder();

    //Initializes message
    void StartMessage();
    //Returns message object created
    DataMessage CompleteMessage();

    //Builds a message from a flattened message string, will build proper message type
    //based on flag and assign it to the Message object
    static int BuildMessageFromFlattened(Message *&message, string flattened_message, unsigned int max_capacity);

    //Setters for message contents
    void SetRecipient(unsigned int recipient);
    void SetSender(unsigned int sender);
    void SetMessageContents(KeyValuePairContainer container);

private:
    unsigned int current_recipient_;
    unsigned int current_sender_;
    KeyValuePairContainer message_contents_;
};

#endif
