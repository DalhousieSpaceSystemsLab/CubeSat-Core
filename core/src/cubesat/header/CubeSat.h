//  CubeSat.h/cc 
//      Basic implementation for the CubeSat class
//

#ifndef LORIS_CUBESAT_CUBESAT_H_
#define LORIS_CUBESAT_CUBESAT_H_

#include <vector>

#include "Messenger.h"
#include "GroundStation.h"
#include "BatteryManager.h"


class CubeSat: public Messenger {
    public:

        CubeSat();

        void SendMessageToGroundStation(const Message &message);
        void ProcessMessage(const Message &message);
        void set_ground_station(const GroundStation &ground_station) { this->ground_station_ = ground_station; };

    private:
        GroundStation ground_station_;
        BatteryManager battery_manager_;
};

#endif
