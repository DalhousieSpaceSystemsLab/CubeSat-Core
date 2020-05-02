#include "KeyValuePairContainer.h"
#include <stdio.h>
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
    this->key_string_pairs_.clear();
}

//TODO Andrew: check if key value pair already exists in the container. If so, overwrite it.
void KeyValuePairContainer::AddKeyValuePair(unsigned int key, float value) {
    this->key_float_pairs_.push_back(KeyFloatPair(key, value));
}

void KeyValuePairContainer::AddKeyValuePair(unsigned int key, int value) {
    this->key_int_pairs_.push_back(KeyIntPair(key, value));
}

void KeyValuePairContainer::AddKeyValuePair(unsigned int key, std::string value) {
    this->key_string_pairs_.push_back(KeyStringPair(key, value));
}

//TODO delete this function? Only use GetKeys and GetFloat
std::vector<int> KeyValuePairContainer::GetKeys() {
    std::vector<int> keys;
    for (std::size_t i = 0; i < this->key_float_pairs_.size(); ++i) {
        keys.push_back(this->key_float_pairs_[i].key());
    }

    for (std::size_t i = 0; i < this->key_int_pairs_.size(); ++i) {
        keys.push_back(this->key_int_pairs_[i].key());
    }

    for (std::size_t i = 0; i < this->key_string_pairs_.size(); ++i) {
        keys.push_back(this->key_string_pairs_[i].key());
    }
    return keys;
}

bool KeyValuePairContainer::ContainsFloatKey(unsigned int key){
	std::vector<int> keys = GetFloatKeys();
	return ContainsKey(key,keys);
}

bool KeyValuePairContainer::ContainsIntKey(unsigned int key){
	std::vector<int> keys = GetIntKeys();
	return ContainsKey(key,keys);
}

//TODO optimize search by having key list being sorted and use search algorithm
bool KeyValuePairContainer::ContainsKey(unsigned int key, std::vector<int> keys){
	for(int i=0;i<keys.size();i++){
		if(keys[i]==key){
			return true;
		}
	}
	return false;
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

std::vector<int> KeyValuePairContainer::GetStringKeys() {
    std::vector<int> keys;
    for (std::size_t i = 0; i < this->key_string_pairs_.size(); ++i) {
        keys.push_back(this->key_string_pairs_[i].key());
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

std::string KeyValuePairContainer::GetString(int key) {
    for(int i=0;i<key_string_pairs_.size();i++){
        if(this->key_string_pairs_[i].key()==key){
            return this->key_string_pairs_[i].value();
        }
    }   
    return "FAIL_CODE";
}

int KeyValuePairContainer::GetAmountofFloatPairs(){
    return this->key_float_pairs_.size();
}

int KeyValuePairContainer::GetAmountofIntPairs(){
    return this->key_int_pairs_.size();
}

int KeyValuePairContainer::GetAmountofStringPairs(){
    return this->key_string_pairs_.size();
}

int KeyValuePairContainer::Flatten(char* msg, int msg_size, unsigned int max_message_size){
    msg_size += FlattenIntPairs(msg, msg_size, max_message_size);
    msg_size += FlattenFloatPairs(msg, msg_size, max_message_size);
    msg_size += FlattenStringPairs(msg, msg_size, max_message_size);
    return msg_size;
}
    
int KeyValuePairContainer::FlattenIntPairs(char* msg, int msg_size, unsigned int max_message_size){
    char integer_string[32];
    std::vector<int> keys = this->GetIntKeys();
    int i = 0;
    //For each int key 
    while(i < keys.size()){
        //Get value in hexidecimal
        sprintf(integer_string, "%x", keys[i]);
        strcat(msg, integer_string);
        //Increase msg_size proportionally
        msg_size += strlen(integer_string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        //Add key value pair to msg
        strcat(msg, "~");
        sprintf(integer_string, "%x", this->GetInt(keys[i]));
        msg_size += strlen(integer_string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
    }
    msg_size += 1;
    if(msg_size > max_message_size){
        throw std::invalid_argument( "Message to large" );
    }
    return msg_size;
}

int KeyValuePairContainer::FlattenFloatPairs(char* msg, int msg_size, unsigned int max_message_size){
    char integer_string[32];
    std::vector<int> keys = this->GetFloatKeys();
    int i = 0;
    //For all float keys
    while(i < keys.size()){
        //read in float value
        sprintf(integer_string, "%x", keys[i]);
       //Increase message size
        msg_size += strlen(integer_string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        //Add key value pair to msg
        strcat(msg, integer_string); 
        strcat(msg, "~");
        sprintf(integer_string, "%f", this->GetFloat(keys[i]));
        msg_size += strlen(integer_string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, integer_string); 
        strcat(msg, "|");
        i++;
    }
    return msg_size;
}

int KeyValuePairContainer::FlattenStringPairs(char* msg, int msg_size, unsigned int max_message_size){
    char integer_string[32];
    std::vector<int> keys = this->GetStringKeys();
    int i = 0;
    //For all float keys
    while(i < keys.size()){
        //read in float value
        sprintf(integer_string, "%x", keys[i]);
       //Increase message size
        msg_size += strlen(integer_string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        //Add key value pair to msg
        strcat(msg, integer_string); 
        strcat(msg, "~");
        char *string = (char*)std::malloc(strlen(this->GetString(keys[i]).c_str()) + 1);
        sprintf(string, "%s", this->GetString(keys[i]).c_str());
        msg_size += strlen(string) + 1;
        if(msg_size > max_message_size){
            throw std::invalid_argument( "Message to large" );
        }
        strcat(msg, string); 
        strcat(msg, "|");
        i++;
    }
    return msg_size;
}
