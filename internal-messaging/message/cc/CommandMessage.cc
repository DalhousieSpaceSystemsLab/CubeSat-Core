#include <iostream>
#include <cstring>
#include "CommandMessage.h"
#include <stdexcept>

CommandMessage::CommandMessage()
:Message()
{
    flag = 1;
}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient)
:Message(sender, recipient)
{  
    flag = 1;
}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, contents)
{
    flag = 1;
}

CommandMessage::CommandMessage(char* flat)
{
    std::cout << "Creating Command Message" << std::endl;
    flag = 1;

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

    // Add Key Value Pairs
    this->contents_.Flatten(msg, message_size);
    message_size += 1;
    if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
    }
    // Add end char
    strcat(msg, "\3");
}
