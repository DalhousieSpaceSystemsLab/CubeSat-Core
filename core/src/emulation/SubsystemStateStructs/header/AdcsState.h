#ifndef ADCS_STATE_H
#define ADCS_STATE_H

#include <iostream>
#include "SubsystemState.h"

using namespace std;

struct AdcsState : SubsystemState {

	float zenith_angle;
	
	float azimuthal_angle;
};

#endif
