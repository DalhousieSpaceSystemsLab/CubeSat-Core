#include <iostream>
#include <cstring>
#include "DataMessage.h"
#include <stdexcept>

bool number_value(const char* string) {
    bool decimal_occurred = false;
    const int string_len = strlen(string);
    for(int i = 0; i < string_len; ++i) {
        if(string[i] == '.' && !decimal_occurred){ //first decimal occurrance
            decimal_occurred = true;
        } else if(decimal_occurred && string[i] == '.') { //any other decimal occurance
            return false;
        } else if(!isdigit(string[i])) { //not a number
            if(i != 0 || string[i] != '-'){ //if the char is not setting the number as a negative
                return false;
            }
        }
    }  
    return true; //string is a number
}

DataMessage::DataMessage()
:Message()
{
    // NULL
}

DataMessage::DataMessage(unsigned int sender, unsigned int recipient)
:Message(sender, recipient)
{  
    //NULL
}
DataMessage::DataMessage(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents)
:Message(sender, recipient, time, contents)
{
    // NULL
}

DataMessage::DataMessage(char* flat)
{
    std::cout << "Creating Message" << std::endl;
    char integer_string[32];
    char hex_string[32];
    char long_string[64];
    memset(integer_string, 0, 32);
    memset(long_string, 0, 64);
    memset(hex_string, 0, 32);
    int i = 0;

    //find sender
    while(flat[i] != '|'){
        sprintf(integer_string, "%c", flat[i]);
        strcat(hex_string, integer_string); 
        i++;
    }
    try {
        this->sender_ = std::stoi(hex_string,nullptr,16);
    } catch (std::exception const &e) {}

    i++;
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);

    //find recipient
    while(flat[i] != '|'){
        sprintf(integer_string, "%c", flat[i]);
        strcat(hex_string, integer_string);
        i++; 
    }
    try {
        this->recipient_ = std::stoi(hex_string,nullptr,16);
    } catch (std::exception const &e) {}

    i++;
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);
    
    //find time sent
    while(flat[i] != '|'){
        sprintf(integer_string, "%c", flat[i]);
        strcat(hex_string, integer_string); 
        i++;
    }
    try {
        this->time_created_ = std::stoi(hex_string,nullptr,16);
    } catch (std::exception const &e) {}
    memset(long_string, 0, 64);
    memset(hex_string, 0, 32);

    //Find key value pairs
    i++;
    int key;
    while(flat[i] != '\3'){
        //Get key
        while(flat[i] != '~'){
            sprintf(integer_string, "%c", flat[i]);
            strcat(hex_string, integer_string); 
            i++;
        }
        i++;
        try {
            key = std::stoi(hex_string,nullptr,16);
        } catch (std::exception const &e) {}
        memset(long_string, 0, 64);
        memset(hex_string, 0, 32);
        //Get value
        while(flat[i] != '|'){
            sprintf(integer_string, "%c", flat[i]);
            strcat(hex_string, integer_string); 
            i++;
        }
        try {
            if(!number_value(hex_string)){
                std::string value = hex_string;
                contents_.AddKeyValuePair(key, value);
            }
            else if(strchr(hex_string, 46)){ //if hex_string contains a '.'
                float value = std::stof(hex_string);
                contents_.AddKeyValuePair(key, value);
            }
            else{
                int value = std::stoul(hex_string,nullptr,16);
                contents_.AddKeyValuePair(key, value);
            }
        } catch (std::exception const &e) {}
        memset(long_string, 0, 64);
        memset(hex_string, 0, 32);
        i++;
    }
}

void DataMessage::flatten(char* msg) {
    int message_size = 0;
    char integer_string[32];
    char long_string[64];

    //Add Sender, Recipient, and Time
    sprintf(integer_string, "%x", this->sender_);
    message_size += strlen(integer_string) + 1;
    if(message_size > 255){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, integer_string); 
    strcat(msg, "|");

    sprintf(integer_string, "%x", this->recipient_);
    message_size += strlen(integer_string) + 1;
    if(message_size > 255){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, integer_string);
    strcat(msg, "|");
    
    sprintf(long_string, "%lx", this->time_created_);
    message_size += strlen(long_string) + 1;
    if(message_size > 255){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, long_string);
    strcat(msg, "|");

    //Add Key Value Pairs
    this->contents_.flatten(msg, message_size);
    message_size += 1;
    if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
    }
    //add end char
    strcat(msg, "\3");
}

std::vector<int> DataMessage::GetRequestKeys(){
	return requests;
}


