#include "PowerEmulator.h"
#include "DataMessage.h"
#include "PowerKeys.h"
#include <iostream>

// Constructor with default hardware setup
PowerEmulator::PowerEmulator() {
    Battery battery1;

    battery1.set_power_level(this->max_power_);
    set_main_battery(battery1);
}

PowerEmulator::PowerEmulator(Battery battery) {
    this->main_battery_ = battery;
}

bool PowerEmulator::ToggleOBCRelay() {
    return this->obc_relay_.toggle();
}

bool PowerEmulator::ToggleRelay(Relay relay) {
    return relay.toggle();
}

float PowerEmulator::GetMainPower() {
    return this->main_battery_.power_level();
}

float PowerEmulator::GetBatteryPower(Battery battery) {
    return battery.power_level();
}

void PowerEmulator::GetState(DataMessage & message) {
   PowerKeys keys;
   const unsigned int battery_level_key=keys.battery_level;
   float battery_level = GetMainPower();
   message.Add(battery_level_key,battery_level);
}

void PowerEmulator::Update(long time) {
    float current_battery_level = this->main_battery_.power_level();
    current_battery_level+=sin(time)*this->max_power_;
    main_battery_.set_power_level(current_battery_level);
}
