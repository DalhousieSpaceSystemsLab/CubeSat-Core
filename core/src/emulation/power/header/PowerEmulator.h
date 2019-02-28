//  PowerEmulator.h/cc
//      Basic implementation for the Battery class
//
//#include "SubsystemEmulator.h"
#include "Battery.h"
#include "Relay.h"

#ifndef LORIS_EMULATION_POWER_POWEREMULATOR_H_
#define LORIS_EMULATION_POWER_POWEREMULATOR_H_

class PowerEmulator {
public:
    PowerEmulator();

    PowerEmulator(Battery battery);

    void set_main_battery( Battery battery ) { this->main_battery_ = battery; };

    bool ToggleOBCRelay();

    float GetMainPower();

private:
    Battery main_battery_;
    Relay obc_relay_;

};

#endif