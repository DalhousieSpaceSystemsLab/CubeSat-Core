
#ifndef LORIS_MESSAGE_KEYSTRINGPAIR_H_
#define LORIS_MESSAGE_KEYSTRINGPAIR_H_

#include "Key.h"
#include <string>

//A key which has an associated value of type int
class KeyStringPair : public Key {
public:
    //Constructors
    KeyStringPair();
    //key - initizlizes key_ in superclass 
    // value - string value associated with key
    KeyStringPair(unsigned int key, std::string value);
    //Getter for value_
    std::string value();
private:
    std::string value_;
};

#endif
