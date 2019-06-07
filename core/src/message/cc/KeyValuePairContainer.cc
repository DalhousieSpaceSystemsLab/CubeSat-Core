#include "KeyValuePairContainer.h"

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

float KeyValuePairContainer::GetFloat(int index) {
    return this->key_float_pairs_[index].value();
}

int KeyValuePairContainer::GetInt(int index) {
    return this->key_int_pairs_[index].value();
}

int KeyValuePairContainer::GetAmountofFloatPairs(){
    return this->key_float_pairs_.size();
}