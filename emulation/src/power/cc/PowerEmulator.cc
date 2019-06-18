#include "PowerEmulator.h"
#include "Message.h"
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

bool PowerEmulator::ToggleRelay(Relay relay) {
    return relay.toggle();
}

float PowerEmulator::GetMainPower() {
    return this->main_battery_.power_level();
}

float PowerEmulator::GetBatteryPower(Battery battery) {
    return battery.power_level();
}

void PowerEmulator::GetState(Message *message) {
   
}

void PowerEmulator::Update(long time) {
    float current_battery_level = this->main_battery_.power_level();
    main_battery_.set_power_level(time*sin(current_battery_level));
}