#include <iostream>
#include <cstring>
#include "Message.h"
#include <stdexcept>

// Return true if string is a number, false if string is not a numver
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

Message::Message(){}

Message::Message(unsigned int sender, unsigned int recipient)
:sender_(sender), recipient_(recipient)
{  
    //NULL
}
Message::Message(unsigned int sender, unsigned int recipient, long time, int childs_flag, KeyValuePairContainer contents)
:sender_(sender), recipient_(recipient), time_created_(time), contents_(contents), flag(childs_flag), capacity(DEFAULT_MESSAGE_CAPACITY)
{
    // NULL
}

Message::Message(char* flat)
{
    // NULL
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

void Message::Add(const unsigned int key, int value){
    this->contents_.AddKeyValuePair(key,value);
}

void Message::Add(const unsigned int key, float value){
    this->contents_.AddKeyValuePair(key,value);
}

void Message::Add(const unsigned int key, std::string value){
    this->contents_.AddKeyValuePair(key,value);
}

float Message::GetFloat(const unsigned int key){
	return contents_.GetFloat(key);
}

int Message::GetInt(const unsigned int key){
	return contents_.GetInt(key);
}

std::string Message::GetString(const unsigned int key){
	return contents_.GetString(key);
}

std::vector<int> Message::GetFloatKeys(){
	return contents_.GetFloatKeys();
}

std::vector<int> Message::GetIntKeys(){
	return contents_.GetIntKeys();
}

std::vector<int> Message::GetStringKeys(){
	return contents_.GetStringKeys();
}

void Message::ToString(char * string, int capacity){
    char long_string[capacity];
    std::vector<int> float_keys = this->contents_.GetFloatKeys();
    std::vector<int> int_keys = this->contents_.GetIntKeys();
    std::vector<int> string_keys = this->contents_.GetStringKeys();

    
    //printf("Printing key int pairs\n");
    //Append int key value pairs
    for(int i=0;i<int_keys.size();i++){
        char integer_string[32];
        int key = int_keys[i];
        int value = this->contents_.GetInt(key);
        sprintf(integer_string, "%d %d",key,value);
        //("Pair %d: %d %d\n",i,key,value);
        
        strcat(long_string, integer_string);
        strcat(long_string,"\n");
    }

    if(int_keys.size()==0){
        //printf("No integer key value pairs\n");
    }

    //printf("Printing key float pairs\n");
    //Append float key value pairs
    for(int i=0;i<float_keys.size();i++){
        char float_string[32];
        int key = float_keys[i];
        float value = this->contents_.GetFloat(key);
        sprintf(float_string, "%d %f",key,value);
        //printf("Pair %d: %d %f\n",i,key,value);

        strcat(long_string, float_string);
        strcat(long_string,"\n");
    }

    if(float_keys.size()==0){
        //printf("No float key value pairs\n");
    }

    //printf("Printing key float pairs\n");
    //Append float key value pairs
    for(int i=0;i<string_keys.size();i++){
        char strings_string[32];
        int key = string_keys[i];
        std::string value = this->contents_.GetString(key);
        sprintf(strings_string, "%d %s",key,value.c_str());
        //printf("Pair %d: %d %f\n",i,key,value);

        strcat(long_string, strings_string);
        strcat(strings_string,"\n");
    }

    if(float_keys.size()==0){
        //printf("No float key value pairs\n");
    }

    for(int i=0; i < capacity; ++i){
        string[i] = long_string[i];
    }

}

int Message::FlattenHeader(char* msg, int msg_size){
    int message_size = msg_size;
    char integer_string[32];
    char long_string[64];

     // Add Sender, Recipient, and Time
    sprintf(integer_string, "%x", this->sender_);
    message_size += strlen(integer_string) + 1;
    if(message_size > capacity){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, integer_string); 
    strcat(msg, section_break);

    sprintf(integer_string, "%x", this->recipient_);
    message_size += strlen(integer_string) + 1;
    if(message_size > capacity){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, integer_string);
    strcat(msg, section_break);

    sprintf(long_string, "%lx", this->time_created_);
    message_size += strlen(long_string) + 1;
    if(message_size > capacity){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, long_string);
    strcat(msg, section_break);

    sprintf(integer_string, "%x", this->flag);
    message_size += strlen(integer_string) + 1;
    if(message_size > capacity){
        throw std::invalid_argument( "Message to large" );
    }
    strcat(msg, integer_string); 
    strcat(msg, section_break);

    return message_size;
}

int Message::BuildHeader(char* flat, int index){
    char integer_string[32];
    char hex_string[32];
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);
    int i = index;

    // Find sender
    while(flat[i] != *section_break){
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

    // Find recipient
    while(flat[i] != *section_break){
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
    
    // Find time sent
    while(flat[i] != *section_break){
        sprintf(integer_string, "%c", flat[i]);
        strcat(hex_string, integer_string); 
        i++;
    }
    try {
        this->time_created_ = std::stoi(hex_string,nullptr,16);
    } catch (std::exception const &e) {}

    i++;
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);
    
    // Find flag
    while(flat[i] != *section_break){
        sprintf(integer_string, "%c", flat[i]);
        strcat(hex_string, integer_string); 
        i++;
    }
    try {
        this->flag = std::stoi(hex_string,nullptr,16);
    } catch (std::exception const &e) {}
    memset(integer_string, 0, 32);
    memset(hex_string, 0, 32);

    return i;
}

int Message::BuildContents(char* flat, int index){
    char integer_string[32];
    char hex_string[32];
    char long_string[64];
    memset(integer_string, 0, 32);
    memset(long_string, 0, 64);
    memset(hex_string, 0, 32);
    int i = index;
    int key;
    while(flat[i] != *message_break){
        //Get key
        while(flat[i] != *pair_break){
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
        // Get value
        while(flat[i] != *section_break){
            sprintf(integer_string, "%c", flat[i]);
            strcat(hex_string, integer_string); 
            i++;
        }
        try {
            if(!number_value(hex_string)){
                std::string value = hex_string;
                contents_.AddKeyValuePair(key, value);
            }
            else if(strchr(hex_string, 46)){ // If hex_string contains a '.'
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
    return i;
}

int Message::GetFlag(){
    return this->flag;
}

void Message::setCapacity(unsigned int max){
    this->capacity = max;
}
unsigned int Message::GetCapacity(){
    return this->capacity;
}