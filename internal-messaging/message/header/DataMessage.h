
#ifndef LORIS_DATAMESSAGE_H
#define LORIS_DATAMESSAGE_H
#include "Message.h"
class DataMessage : public Message{
    public:
        //Constructors
    	DataMessage();
        //Build message with sender and reciepient, time created will be set to the current date and time
        DataMessage(unsigned int sender, unsigned int recipient);
        //Total constructor building message with all private members 
        DataMessage(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
        //Builds message from character array output of flatten method
        DataMessage(char* flat);

        // Flattens message into a compressed character array that can be parsed by the Message(char* flat) constructor
        // msg - pointer to char array with a minimum size of 256 bytes.
        void flatten(char* msg) override;

        std::vector<int> GetRequestKeys();

    private:
        std::vector<int> requests;
};

#endif