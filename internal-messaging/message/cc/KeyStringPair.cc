#include "KeyStringPair.h"

KeyStringPair::KeyStringPair()
{
    // Null
}

KeyStringPair::KeyStringPair(unsigned int key, std::string value) :
    Key(key), value_(value)
{

}

std::string KeyStringPair::value()
{
    return this->value_;
}