//  BatteryManager.h
//      Basic implementation for the BatteryManager class
//

#ifndef LORIS_BATTERIES_BATTERYMANAGER_H_
#define LORIS_BATTERIES_BATTERYMANAGER_H_

#include <vector>

#include "Battery.h"

class BatteryManager {
    public:
        BatteryManager();
        BatteryManager(std::vector<Battery> batteries);
        void CalcAvgBatteryLevel();

        std::vector<Battery*> batteries() { return this->batteries_; }
        float avg_battery_level() {
		CalcAvgBatteryLevel(); 
		return this->avg_battery_level_; }

    private:
        std::vector<Battery*> batteries_;
        
        float avg_battery_level_;

};

#endif
