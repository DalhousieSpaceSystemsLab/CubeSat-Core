#include <iostream>
#include <cstring>
#include "CommandMessage.h"
#include <stdexcept>

CommandMessage::CommandMessage()
:CommandMessage(0,0)
{}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient)
:CommandMessage(sender, recipient, 0, KeyValuePairContainer())
{}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, 200, contents)
{}

CommandMessage::CommandMessage(char* flat)
{
    std::cout << "Creating Command Message" << std::endl;
    // Find sender, recipient, and time
    int i = BuildHeader(flat, 0);
    i++;
    // Skip over flag
    while(flat[i] != '|'){
        i++;
    }
    i++;
    // Find key value pairs
    i = BuildContents(flat, i);
}

void CommandMessage::Flatten(char* msg) {
    int message_size = 0;
    
    // Add sender, recipient, time, and flag
    FlattenHeader(msg, message_size);
    if(flag != 200){
        throw std::invalid_argument("Flattened message had incorrect flag for a DataMessage");
    }

    // Add Key Value Pairs
    this->contents_.Flatten(msg, message_size);
    message_size += 1;
    if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
    }
    // Add end char
    strcat(msg, "\3");
}
