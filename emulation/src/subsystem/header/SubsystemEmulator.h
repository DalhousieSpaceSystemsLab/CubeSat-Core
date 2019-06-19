//  SubsystemEmulator.h
//      Basic implementation for the abstract SubsystemEmulator class
//
#ifndef LORIS_EMULATION_POWER_SUBSYSTEM_H_
#define LORIS_EMULATION_POWER_SUBSYSTEM_H_

#include "Message.h"

class SubsystemEmulator {
public:
    SubsystemEmulator();

    virtual void Update(long time) = 0;

    virtual void GetState(Message & message) = 0;

private:

};

#endif