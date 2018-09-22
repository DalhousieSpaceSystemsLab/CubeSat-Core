//  BatteryManager.h/cc 
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

        std::vector<Battery> getBatteries() { return this->batteries_; }

    private:
        std::vector<Battery> batteries_;

};

#endif