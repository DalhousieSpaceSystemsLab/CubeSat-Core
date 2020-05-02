
#ifndef LORIS_COMMANDMESSAGE_H
#define LORIS_COMMANDMESSAGE_H
#include "Message.h"
using std::string;

// Message used to send commands to a subsystem
// CommandMessage format:
// sender|recipient|time_created|flag|commandkey~value|commandkey~value|\0
// flag = 200
class CommandMessage : public Message{
    public:
        //Constructors
        //Build message with sender and reciepient, time created will be set to the current date and time
        CommandMessage(unsigned int sender = 0, unsigned int recipient = 0);
        //Total constructor building message with all private members 
        CommandMessage(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
        //Builds message from character array output of flatten method
        CommandMessage(char* flat);
        CommandMessage(char* flat, unsigned int max_size);

        CommandMessage(string flat);
        CommandMessage(string flat, unsigned int max_size);

        //Flattens message into a compressed character array that can be parsed by the Message(char* flat) constructor
        //msg - pointer to char array
        //msg should have atleast the capacity (of base class Message) worth of bytes allocated
        void Flatten(char* msg) override;

    private: 
        /**
         * Takes a character array, extracts data, and assigns it to class variables
         * This function assumes that flat is the "flattened" version of a pre-existing DataMessage
         * @param flat
         * @return
         */
        int BuildFromCharacters(char * flat);

};

#endif
