#include "BatteryManager.h"

// Null constructor for BatteryManager class
//
BatteryManager::BatteryManager() {
    // Null 
}

// Constructor for BatteryManager class
BatteryManager::BatteryManager(std::vector<Battery> batteries) {
    this->batteries_=batteries;
}