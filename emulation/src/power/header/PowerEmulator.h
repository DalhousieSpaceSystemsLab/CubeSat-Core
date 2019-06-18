//  PowerEmulator.h
//      Basic implementation for the Battery class
//
#include "SubsystemEmulator.h"
#include "Battery.h"
#include "Relay.h"
#include "Message.h"
#include <math.h>

#ifndef LORIS_EMULATION_POWER_POWEREMULATOR_H_
#define LORIS_EMULATION_POWER_POWEREMULATOR_H_

class PowerEmulator : public SubsystemEmulator {
public:
    PowerEmulator();

    PowerEmulator(Battery battery);

    void set_main_battery( Battery battery ) { this->main_battery_ = battery; };

    bool ToggleOBCRelay();

    bool ToggleRelay(Relay relay);

    float GetMainPower();

    float GetBatteryPower(Battery battery);

    void GetState(Message * message);

    void Update(long time);

private:
    Battery main_battery_;
    Relay obc_relay_;

};

#endif
