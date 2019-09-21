
#ifndef LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_
#define LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_

#include "KeyIntPair.h"
#include "KeyFloatPair.h"
#include "KeyStringPair.h"
#include "KeyValuePairContainerInterface.h"
#include <vector>
#include <string.h>

//Key Value Pair
class KeyValuePairContainer : public KeyValuePairContainerInterface {
public:

    //Constructor && Destructor
    KeyValuePairContainer();
    ~KeyValuePairContainer();

    //Add various Key Value pair types
    void AddKeyValuePair(unsigned int key, float value);
    void AddKeyValuePair(unsigned int key, int value);
    void AddKeyValuePair(unsigned int key, std::string value);

    //Gets lists of Keys, or pair type specific keys as vectors
    std::vector<int> GetKeys();
    std::vector<int> GetFloatKeys();
    std::vector<int> GetIntKeys();
    std::vector<int> GetStringKeys();
    
    //Gets a particular value based on the key
    float GetFloat(int key);
    int GetInt(int key);
    std::string GetString(int key);

    //Gets total amount of various types of pairs
    int GetAmountofFloatPairs();
    int GetAmountofIntPairs();
    int GetAmountofStringPairs();

    bool ContainsFloatKey(unsigned int key);
    bool ContainsIntKey(unsigned int key);


    //Flattens for message sending - returns new msg_size (CANNOT EXCEED message_max_size)
    int Flatten(char* msg, int msg_size, unsigned int message_max_size);
    int FlattenIntPairs(char* msg, int msg_size, unsigned int message_max_size);
    int FlattenFloatPairs(char* msg, int msg_size, unsigned int message_max_size);
    int FlattenStringPairs(char* msg, int msg_size, unsigned int message_max_size);
private:
    std::vector<KeyIntPair> key_int_pairs_;
    std::vector<KeyFloatPair> key_float_pairs_;
    std::vector<KeyStringPair> key_string_pairs_;
    bool ContainsKey(unsigned int key, std::vector<int> keys);
};

#endif
