
/*Resources:
https://stackoverflow.com/questions/16543519/serialization-of-struct
https://beej.us/guide/bgnet/html/multi/advanced.html#serialization
*/

#ifndef POWER_SERILIAZER
#define POWER_SERILIAZER
#include <iostream>
#include "PowerState.h"

// takes data, a location large enough to store the PowerState Struct in memory, and stores all its members
void serialize(PowerState* newState, float *data)
{
    float *q = data; // sets q to data in memory
    *q = newState->main_battery_level;  q++;    // store main_battery_level and increment
    *q = newState->obc_relay_toggled;   q++;    // same with obc...
}

// passes same data place in mem as before and a container to store the de serilized data in
void deserialize(float *data, PowerState* newState)
{
    float *q = data;    
    newState->main_battery_level = *q;  q++;    // places contets of q for main_batterey... into the passed empty container
    newState->obc_relay_toggled = *q;   q++;    // same for the bool obc...
}

#endif


