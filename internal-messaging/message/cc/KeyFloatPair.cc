#include "KeyFloatPair.h"

KeyFloatPair::KeyFloatPair()
{
    // Null
}

KeyFloatPair::KeyFloatPair(unsigned int key, float value) :
        Key(key), value_(value)
{

}

float KeyFloatPair::value()
{
    return this->value_;
}