#ifndef ADCS_EMULATOR_H
#define ADCS_EMULATOR_H

#include "AttitudeSensor.h"
#include <iostream>
using namespace std;

class AdcsEmulator
{
    private:

        AttitudeSensor attitude_sensor_;
        
    public:

        void set_orientation (float azimuthal_angle, float zenith_angle);

        float GetAzimuthalAngle (void) const;
        
        float GetZenithAngle (void) const;
};

#endif
