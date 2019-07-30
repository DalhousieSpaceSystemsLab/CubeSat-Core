#include <iostream>
#include <cstring>
#include "Message.h"
#include <stdexcept>

Message::Message(){
	this->time_created_=0;
}

Message::Message(unsigned int sender, unsigned int recipient): sender_(sender), recipient_(recipient)
{  
    //NULL
}
Message::Message(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents):
        sender_(sender), recipient_(recipient), time_created_(time), contents_(contents)
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
