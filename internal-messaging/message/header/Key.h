
#ifndef LORIS_MESSAGE_KEY_H_
#define LORIS_MESSAGE_KEY_H_

//Int value used to get a value in a KeyValuePairContainer
class Key {
public:
    //Constructors
    Key();
    //key - int value used to call the value
    Key(unsigned int key);
    //getter for key
    unsigned int key();
private:
    unsigned int key_;
};

#endif
