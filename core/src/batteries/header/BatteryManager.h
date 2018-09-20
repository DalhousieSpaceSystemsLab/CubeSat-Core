#ifndef LORIS_BATTERIES_BATTERYMANAGER_H_
#define LORIS_BATTERIES_BATTERYMANAGER_H_

#include <vector>

#include "Battery.h"

class BatteryManager {
    public:
        BatteryManager();

        std::vector<Battery> getBatteries() { return batteries; }

    private:
        std::vector<Battery> batteries;

};

#endif