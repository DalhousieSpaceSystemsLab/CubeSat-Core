#include <iostream>
#include <cstring>
#include "CommandMessage.h"
#include <stdexcept>
#include <string>

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient)
:CommandMessage(sender, recipient, 0, KeyValuePairContainer())
{}

CommandMessage::CommandMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, 200, contents)
{}

CommandMessage::CommandMessage(char* flat) :
    CommandMessage(flat, DEFAULT_MESSAGE_CAPACITY){
}

CommandMessage::CommandMessage(char* flat, unsigned int max_size)
{
    std::cout << "Creating Command Message" << std::endl;
    setCapacity(max_size);
    // Find sender, recipient, and time
    int i = BuildHeader(flat, 0);
    i++;
    // Skip over flag
    while(flat[i] != *section_break){
        i++;
    }
    i++;
    // Find key value pairs
    i = BuildContents(flat, i);
}

CommandMessage::CommandMessage(string flat) :
    CommandMessage(flat, DEFAULT_MESSAGE_CAPACITY){
}

CommandMessage::CommandMessage(string flat, unsigned int max_size) {
	setCapacity(max_size);
    char cstr[flat.size() + 1];
	std::strcpy(cstr, flat.c_str());	// or pass &s[0]
	BuildFromCharacters(cstr);
}

void CommandMessage::Flatten(char* msg) {
    int message_size = 0;
    
    // Add sender, recipient, time, and flag
    FlattenHeader(msg, message_size);
    if(flag != 200){
        throw std::invalid_argument("Flattened message had incorrect flag for a DataMessage");
    }

    // Add Key Value Pairs
    this->contents_.Flatten(msg, message_size, capacity);
    message_size += 1;
    if(message_size > capacity){
            throw std::invalid_argument( "Message to large" );
    }
    // Add end char
    strcat(msg, message_break);
}

int CommandMessage::BuildFromCharacters(char *flat) {
	std::cout << "Creating Data Message" << std::endl;
	char integer_string[32];
	char hex_string[32];
	memset(integer_string, 0, 32);
	memset(hex_string, 0, 32);
	// Find sender, recipient, time and flag
	int i = BuildHeader(flat, 0);
	i++;

	//TODO Spencer, should the flag be parsed in parent class Message? - Andrew
	// Skip over flag
	while (flat[i] != *section_break) {
		i++;
	}

	// Find key value pairs
	i++;
	i = BuildContents(flat, i);
	return 1;
}