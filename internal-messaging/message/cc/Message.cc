#include <iostream>
#include <cstring>
#include "Message.h"
#include <stdexcept>


Message::Message(unsigned int sender, unsigned int recipient): sender_(sender), recipient_(recipient)
{  

}
Message::Message(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents):
        sender_(sender), recipient_(recipient), time_created_(time), contents_(contents)
{
    // NULL
}

Message::Message(char* flat)
{
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

    i++;
    int key;
    while(flat[i] != '\3'){
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
        while(flat[i] != '|'){
            sprintf(integer_string, "%c", flat[i]);
            strcat(hex_string, integer_string); 
            i++;
        }
        try {
            if(strchr(hex_string, 46)){ //if hex_string contains a '.'
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

void Message::flatten(char* msg){
    //TODO ensure no buffer overflow
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
    std::vector<int> keys = contents_.GetKeys();
    int floats = contents_.GetAmountofFloatPairs();
    int i = 0;
    int n = 0;
    while(i < floats){
        sprintf(integer_string, "%x", keys[i]);
        message_size += strlen(integer_string) + 1;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "~");
        sprintf(integer_string, "%f", contents_.GetFloat(i));
        message_size += strlen(integer_string) + 1;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
    }
    while(i < keys.size()){
        sprintf(integer_string, "%x", keys[i]);
        strcat(msg, integer_string); 
        message_size += strlen(integer_string) + 1;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, "~");
        sprintf(integer_string, "%x", contents_.GetInt(n));
        message_size += strlen(integer_string) + 1;
        if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
        n++;
    }
    message_size += 1;
    if(message_size > 255){
            throw std::invalid_argument( "Message to large" );
    }
    //add end char
    strcat(msg, "\3");
}

KeyValuePairContainer Message::GetMessageContents() const
{
	return this->contents_;
}

unsigned int Message::GetSender() const 
{
	return this->sender_;
}

unsigned int Message::GetRecipient() const
{
	return this->recipient_;
}

long Message::GetTimeCreated() const
{
	return this->time_created_;
}

void Message::SetSender(unsigned int sender_)
{
	this->sender_ = sender_;
}


void Message::SetRecipient(unsigned int recipient_)
{
	this->recipient_ = recipient_;
}

void Message::SetTimeCreated(long time_created_)
{
	this->time_created_ = time_created_;
}

void Message::Add(unsigned int key, int value){
    this->contents_.AddKeyValuePair(key,value);
}

void Message::Add(unsigned int key, float value){
    this->contents_.AddKeyValuePair(key,value);
}


void Message::ToString(char * string, int capacity){
    char long_string[capacity];
    std::vector<int> float_keys = this->contents_.GetFloatKeys();
    std::vector<int> int_keys = this->contents_.GetIntKeys();
    
    printf("Printing key int pairs\n");
    //Append int key value pairs
    for(int i=0;i<int_keys.size();i++){
        char integer_string[32];
        int key = int_keys[i];
        int value = this->contents_.GetInt(key);
        sprintf(integer_string, "%d %d",key,value);
        printf("Pair %d: %d %d\n",i,key,value);
        
        strcat(long_string, integer_string);
        strcat(long_string,"\n");
    }

    if(int_keys.size()==0){
        printf("No integer key value pairs\n");
    }

    printf("Printing key float pairs\n");
    //Append float key value pairs
    for(int i=0;i<float_keys.size();i++){
        char float_string[32];
        int key = float_keys[i];
        float value = this->contents_.GetFloat(key);
        sprintf(float_string, "%d %f",key,value);
        printf("Pair %d: %d %f\n",i,key,value);

        strcat(long_string, float_string);
        strcat(long_string,"\n");
    }

    if(float_keys.size()==0){
        printf("No float key value pairs\n");
    }

    for(int i=0; i < capacity; ++i){
        string[i] = long_string[i];
    }

}
