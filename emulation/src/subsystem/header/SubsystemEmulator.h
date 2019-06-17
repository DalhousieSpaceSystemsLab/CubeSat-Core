//  SubsystemEmulator.h
//      Basic implementation for the abstract SubsystemEmulator class
//
#ifndef LORIS_EMULATION_POWER_SUBSYSTEM_H_
#define LORIS_EMULATION_POWER_SUBSYSTEM_H_

#include "SubsystemState.h"

class SubsystemEmulator {
public:
    SubsystemEmulator();

    virtual void Update(long time);

    virtual SubsystemState GetState();

private:
    SubsystemState state_;
};

#endif