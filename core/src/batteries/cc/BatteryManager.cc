#include "BatteryManager.h"

// Null constructor for BatteryManager class
//
BatteryManager::BatteryManager() {
    
    for(int i=0 ; i<4 ; i++) {
        this->batteries_.push_back(new Battery());
        batteries_[i]->set_power_level(0.0);
    }

}

// Constructor for BatteryManager class
BatteryManager::BatteryManager(std::vector<Battery> batteries) {
    // Null
}

//
void BatteryManager::CalcAvgBatteryLevel() {

    int size;
    float sum=0;

    size = this->batteries_.size();
    for(int i=0 ; i<size ; i++) { 
        sum += this->batteries_[i]->power_level(); 
    }

    this->avg_battery_level_=(sum/size);

}