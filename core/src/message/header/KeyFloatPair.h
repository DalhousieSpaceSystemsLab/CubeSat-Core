
#ifndef LORIS_MESSAGE_KEYFLOATPAIR_H_
#define LORIS_MESSAGE_KEYFLOATPAIR_H_

#include "Key.h"

class KeyFloatPair : public Key {
public:
    KeyFloatPair();
    KeyFloatPair(unsigned int key, float value);
    float value();
private:
    float value_;
};

#endif
