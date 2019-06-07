#ifndef POWERSTATE
#define POWERSTATE

#include <iostream>

typedef struct PowerState
{
	float main_battery_level;
	bool obc_relay_toggled;
	
}PowerState;

#endif
