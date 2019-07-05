
#ifndef LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_
#define LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_

#include "KeyIntPair.h"
#include "KeyFloatPair.h"
#include "KeyValuePairContainerInterface.h"
#include <vector>

class KeyValuePairContainer : public KeyValuePairContainerInterface {
public:

    //Constructor && Destructor
    KeyValuePairContainer();
    ~KeyValuePairContainer();

    //Add various Key Value pair types
    void AddKeyValuePair(unsigned int key, float value);
    void AddKeyValuePair(unsigned int key, int value);

    //Gets lists of Keys, or pair type specific keys as vectors
    std::vector<int> GetKeys();
    std::vector<int> GetFloatKeys();
    std::vector<int> GetIntKeys();
    
    //Gets a particular value based on the key
    float GetFloat(int key);
    int GetInt(int key);

    //Gets total amount of various types of pairs
    int GetAmountofFloatPairs();
    int GetAmountofIntPairs();

    //flattens for message sending - returns new msg_size (CANNOT EXCEED 255)
    int flatten(char* msg, int msg_size);
    int flattenIntPairs(char* msg, int msg_size);
    int flattenFloatPairs(char* msg, int msg_size);
private:
    std::vector<KeyIntPair> key_int_pairs_;
    std::vector<KeyFloatPair> key_float_pairs_;
};

#endif
