#include "PowerEmulator.h"
//#include "../header/PowerEmulator.h"

// Null constructor for PowerEmulator class
PowerEmulator::PowerEmulator() {
    // Null
}

PowerEmulator::PowerEmulator(Battery battery) {
    this->main_battery_ = battery;
}

bool PowerEmulator::ToggleOBCRelay() {
    return this->obc_relay_.toggle();
}

float PowerEmulator::GetMainPower() {
    return this->main_battery_.power_level();
}