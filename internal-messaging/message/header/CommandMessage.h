
#ifndef LORIS_COMMANDMESSAGE_H
#define LORIS_COMMANDMESSAGE_H
#include "Message.h"

// Message used to send commands to a subsystem
// CommandMessage format:
// sender|recipient|time_created|flag|commandkey~value|commandkey~value|\0
// flag = 1
class CommandMessage : public Message{
    public:
        //Constructors
    	CommandMessage();
        //Build message with sender and reciepient, time created will be set to the current date and time
        CommandMessage(unsigned int sender, unsigned int recipient);
        //Total constructor building message with all private members 
        CommandMessage(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
        //Builds message from character array output of flatten method
        CommandMessage(char* flat);

        // Flattens message into a compressed character array that can be parsed by the Message(char* flat) constructor
        // msg - pointer to char array with a minimum size of 256 bytes.
        void Flatten(char* msg) override;
};

#endif
