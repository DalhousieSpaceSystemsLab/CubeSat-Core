//  SubsystemEmulator.h
//      Basic implementation for the abstract SubsystemEmulator class
//
#ifndef LORIS_EMULATION_POWER_SUBSYSTEM_H_
#define LORIS_EMULATION_POWER_SUBSYSTEM_H_

#include "SubsystemState.h"

class SubsystemEmulator {
public:
    SubsystemEmulator();

    void Update(long time);

    SubsystemState GetState();

private:
    SubsystemState state_;
};

#endif