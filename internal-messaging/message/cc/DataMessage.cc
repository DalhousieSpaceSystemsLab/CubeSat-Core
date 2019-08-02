#include <iostream>
#include <cstring>
#include "DataMessage.h"
#include <stdexcept>

DataMessage::DataMessage()
:DataMessage(0,0)
{}

DataMessage::DataMessage(unsigned int sender, unsigned int recipient)
:DataMessage(sender, recipient, 0, KeyValuePairContainer())
{}

DataMessage::DataMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, 100, contents)
{}

DataMessage::DataMessage(char* flat)
{
    std::cout << "Creating Data Message" << std::endl;
    char integer_string[32];
    char hex_string[32];
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);

    // Find sneder, recipient, time and flag
    int i = BuildHeader(flat, 0);
    i++;
    // Skip over flag
    while(flat[i] != '|'){
        i++;
    }
    i++;
    // Find Requests
    while(flat[i] != '|'){
        // Get next request
        while(flat[i] != '-' && flat[i] != '|'){
            sprintf(integer_string, "%c", flat[i]);
            strcat(hex_string, integer_string); 
            i++;
        }
        try {
            int request = std::stoi(hex_string,nullptr,16);
            requests.push_back(request);
        } catch (std::exception const &e) {}
        memset(integer_string, 0, 32);
        memset(hex_string, 0, 32);
        if(flat[i] != '|'){
            i++;
        };
    }

    // Find key value pairs
    i++;
    i = BuildContents(flat, i);
}

void DataMessage::Flatten(char* msg) {
    int message_size = 0;
    char integer_string[32];

    // Add sender, recipient, time, and flag
    FlattenHeader(msg, message_size);
    if(flag != 100){
        throw std::invalid_argument("Flattened message had incorrect flag for a DataMessage");
    }
    
    // If there are no requests, only a | will be added
    if(requests.size() == 0){
        message_size++;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
    }

    // Add requests
    for(int i = 0; i < requests.size(); i++){
        sprintf(integer_string, "%x", requests.at(i));
        message_size += strlen(integer_string) + 1;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        if(i == requests.size() - 1){
            strcat(msg, integer_string);
        } else {
            strcat(msg, integer_string);
            strcat(msg, "-");
        }
    }
    strcat(msg, "|");

    // Add Key Value Pairs
    this->contents_.Flatten(msg, message_size);
    message_size += 1;
    if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
    }
    // Add end char
    strcat(msg, "\3");
}

void DataMessage::AddRequest(int request){
    requests.push_back(request);
}

std::vector<int> DataMessage::GetRequests(){
	return requests;
}
