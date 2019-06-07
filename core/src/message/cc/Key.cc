#include "Key.h"

Key::Key()
{
    // Null
}

Key::Key(unsigned int key) :
    key_(key)
{

}

unsigned int Key::key()
{
    return this->key_;
}
