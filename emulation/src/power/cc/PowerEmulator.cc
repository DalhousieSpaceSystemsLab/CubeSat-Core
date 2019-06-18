#include "PowerEmulator.h"
#include "Message.h"
#include "PowerKeys.h"

// Constructor with default hardware setup
PowerEmulator::PowerEmulator() {
    Battery battery1;

    battery1.set_power_level(50);
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

void PowerEmulator::GetState(Message * message) {
   Message msg = * message;
   unsigned int battery_level_key = PowerKeys::battery_level;
   msg.Add(battery_level_key,GetMainPower());
}

void PowerEmulator::Update(long time) {
    float current_battery_level = this->main_battery_.power_level();
    main_battery_.set_power_level(time*sin(current_battery_level));
}