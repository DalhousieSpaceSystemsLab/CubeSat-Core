#include "KeyValuePairContainer.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>

//TODO this is just temporary. Need to have some way to indicate a function failed. 
const int FAIL_CODE = -1000;

KeyValuePairContainer::KeyValuePairContainer()
{
    // Null
}

KeyValuePairContainer::~KeyValuePairContainer()
{
    this->key_float_pairs_.clear();
    this->key_int_pairs_.clear();
}

void KeyValuePairContainer::AddKeyValuePair(unsigned int key, float value) {
    this->key_float_pairs_.push_back(KeyFloatPair(key, value));
}

void KeyValuePairContainer::AddKeyValuePair(unsigned int key, int value) {
    this->key_int_pairs_.push_back(KeyIntPair(key, value));
}

//TODO delete this function. Only use GetKeys and GetFloat
std::vector<int> KeyValuePairContainer::GetKeys() {
    std::vector<int> keys;
    for (std::size_t i = 0; i < this->key_float_pairs_.size(); ++i) {
        keys.push_back(this->key_float_pairs_[i].key());
    }

    for (std::size_t i = 0; i < this->key_int_pairs_.size(); ++i) {
        keys.push_back(this->key_int_pairs_[i].key());
    }
    return keys;
}
std::vector<int> KeyValuePairContainer::GetIntKeys() {
    std::vector<int> keys;
    for (std::size_t i = 0; i < this->key_int_pairs_.size(); ++i) {
        keys.push_back(this->key_int_pairs_[i].key());
    }
    return keys;
}

std::vector<int> KeyValuePairContainer::GetFloatKeys() {
    std::vector<int> keys;

    for (std::size_t i = 0; i < this->key_float_pairs_.size(); ++i) {
        keys.push_back(this->key_float_pairs_[i].key());
    }
    return keys;
}

//TODO keys must be unsigned integers.
float KeyValuePairContainer::GetFloat(int key) {
    for(int i=0;i<key_float_pairs_.size();i++){
        if(this->key_float_pairs_[i].key()==key){
            return this->key_float_pairs_[i].value();
        }
    }   
    return (float)FAIL_CODE;
}

int KeyValuePairContainer::GetInt(int key) {
    for(int i=0;i<key_int_pairs_.size();i++){
        if(this->key_int_pairs_[i].key()==key){
            return this->key_int_pairs_[i].value();
        }
    }   
    return FAIL_CODE;
}

int KeyValuePairContainer::GetAmountofFloatPairs(){
    return this->key_float_pairs_.size();
}

int KeyValuePairContainer::GetAmountofIntPairs(){
    return this->key_int_pairs_.size();
}

int KeyValuePairContainer::flatten(char* msg, int msg_size){
    msg_size = flattenIntPairs(msg, msg_size);
    msg_size = flattenFloatPairs(msg, msg_size);
    return msg_size;
}
    
int KeyValuePairContainer::flattenIntPairs(char* msg, int msg_size){
    char integer_string[32];
    std::vector<int> keys = this->GetIntKeys();
    int i = 0;
    while(i < keys.size()){
        sprintf(integer_string, "%x", keys[i]);
        strcat(msg, integer_string);
        msg_size += strlen(integer_string) + 1;
        if(msg_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, "~");
        sprintf(integer_string, "%x", this->GetInt(keys[i]));
        msg_size += strlen(integer_string) + 1;
        if(msg_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
    }
    msg_size += 1;
    if(msg_size > 255){
        throw std::invalid_argument( "Message to large" );
    }
    return msg_size;
}

int KeyValuePairContainer::flattenFloatPairs(char* msg, int msg_size){
    char integer_string[32];
    std::vector<int> keys = this->GetFloatKeys();
    int i = 0;
    while(i < keys.size()){
        sprintf(integer_string, "%x", keys[i]);
        msg_size += strlen(integer_string) + 1;
        if(msg_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "~");
        sprintf(integer_string, "%f", this->GetFloat(keys[i]));
        msg_size += strlen(integer_string) + 1;
        if(msg_size > 255){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
    }
    return msg_size;
}
