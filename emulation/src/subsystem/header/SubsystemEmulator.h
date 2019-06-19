//  SubsystemEmulator.h
//      Basic implementation for the abstract SubsystemEmulator class
//
#ifndef LORIS_EMULATION_POWER_SUBSYSTEM_H_
#define LORIS_EMULATION_POWER_SUBSYSTEM_H_

#include "Message.h"

class SubsystemEmulator {
public:
    SubsystemEmulator();

    virtual void Update(long time);

    virtual void GetState(Message* message);

private:

};

#endif