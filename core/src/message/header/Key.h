
#ifndef LORIS_MESSAGE_KEY_H_
#define LORIS_MESSAGE_KEY_H_

class Key {
public:
    Key();
    Key(unsigned int key);
    unsigned int key();
private:
    unsigned int key_;
};

#endif
