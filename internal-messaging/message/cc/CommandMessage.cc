#include <iostream>
#include <cstring>
#include "CommandMessage.h"
#include <stdexcept>

CommandMessage::CommandMessage()
:Message()
{
    // NULL
}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient)
:Message(sender, recipient)
{  
    //NULL
}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, contents)
{
    // NULL
}

CommandMessage::CommandMessage(char* flat)
{
    std::cout << "Creating Command Message" << std::endl;

    // Find sneder, recipient, and time
    int i = BuildHeader(flat, 0);

    // Find key value pairs
    i++;
    i = BuildContents(flat, i);
}

void CommandMessage::Flatten(char* msg) {
    int message_size = 0;
    char integer_string[32];

    // Add sender, recipient, and time
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
