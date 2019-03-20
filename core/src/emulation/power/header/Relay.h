//  Relay.h/cc
//      Basic implementation for the Relay class
//
#ifndef LORIS_EMULATION_POWER_RELAY_H_
#define LORIS_EMULATION_POWER_RELAY_H_

class Relay {
public:
    Relay();

    bool toggle();

private:
    bool toggled_;

};

#endif