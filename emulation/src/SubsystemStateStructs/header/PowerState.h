#ifndef POWER_STATE_H
#define POWER_STATE_H

#include <iostream>
#include "SubsystemState.h"

using namespace std;

struct PowerState : SubsystemState {

	float main_battery_level;
	
	bool obc_relay_toggled;
};

#endif
