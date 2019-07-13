
#ifndef LORIS_MESSAGE_KEYFLOATPAIR_H_
#define LORIS_MESSAGE_KEYFLOATPAIR_H_

#include "Key.h"

//A key which has an associated value of type float
class KeyFloatPair : public Key {
public:
    //Constructors
    KeyFloatPair();
    //key - initizlizes key_ in superclass 
    // value - float value associated with key
    KeyFloatPair(unsigned int key, float value);
    //Getter for value
    float value();
private:
    float value_;
};

#endif
