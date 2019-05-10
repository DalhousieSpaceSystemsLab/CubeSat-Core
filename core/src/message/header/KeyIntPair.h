
#ifndef LORIS_MESSAGE_KEYINTPAIR_H_
#define LORIS_MESSAGE_KEYINTPAIR_H_

#include "Key.h"

class KeyIntPair : public Key {
public:
    KeyIntPair();
    KeyIntPair(unsigned int key, int value);
    int value();
private:
    int value_;
};

#endif
