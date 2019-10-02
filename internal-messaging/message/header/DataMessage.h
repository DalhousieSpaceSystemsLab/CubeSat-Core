
#ifndef LORIS_DATAMESSAGE_H
#define LORIS_DATAMESSAGE_H
#include "Message.h"
using std::string;

// Message used to collect and/or request information from a subsystem
// DataMessage format:
// sender|recipient|time_created|flag|request-request-request|key~value|key~value|\0
// flag = 100

static const char* request_break = "-";

class DataMessage : public Message{
    public:
        //Constructors
        //Build message with sender and reciepient, time created will be set to the current date and time
        DataMessage(unsigned int sender = 0, unsigned int recipient = 0);
        //Total constructor building message with all private members 
        DataMessage(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
        //Builds message from character array output of flatten method
        DataMessage(char* flat);
        DataMessage(char* flat, unsigned int max_size);
        //Builds message from string output of flatten method
        DataMessage(string flat);
        DataMessage(string flat, unsigned int max_size);


        // Flattens message into a compressed character array that can be parsed by the Message(char* flat) constructor
        // msg - pointer to char array
        
        void Flatten(char* msg) override;

        // Methods for adding and collecting requests
        void AddRequest(int request);
        std::vector<int>& GetRequests();

        bool HasRequests();
    private:
        std::vector<int> requests;
        /**
         * Takes a character array, extracts data, and assigns it to class variables
         * This function assumes that flat is the "flattened" version of a pre-existing DataMessage
         * @param flat
         * @return
         */
        int BuildFromCharacters(char * flat);
};

#endif
