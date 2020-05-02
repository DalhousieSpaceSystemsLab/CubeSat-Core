
#ifndef LORIS_MESSAGE_KEYINTPAIR_H_
#define LORIS_MESSAGE_KEYINTPAIR_H_

#include "Key.h"

//A key which has an associated value of type int
class KeyIntPair : public Key {
public:
    //Constructors
    KeyIntPair();
    //key - initizlizes key_ in superclass 
    // value - int value associated with key
    KeyIntPair(unsigned int key, int value);
    //Getter for value_
    int value();
private:
    int value_;
};

#endif
