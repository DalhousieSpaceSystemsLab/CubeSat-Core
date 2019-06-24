#include "KeyIntPair.h"

KeyIntPair::KeyIntPair()
{
    // Null
}

KeyIntPair::KeyIntPair(unsigned int key, int value) :
    Key(key), value_(value)
{

}

int KeyIntPair::value()
{
    return this->value_;
}