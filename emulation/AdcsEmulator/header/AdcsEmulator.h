#ifndef ADCS_EMULATOR_H
#define ADCS_EMULATOR_H

#include "AttitudeSensor.h"
#include "AdcsState.h"
#include "SubsystemEmulator.h"
#include <iostream>
#include <math.h>
using namespace std;

class AdcsEmulator : public SubsystemEmulator
{
    private:

        AttitudeSensor attitude_sensor_;
        AdcsState state_;
        
    public:

        void set_orientation (float azimuthal_angle, float zenith_angle);

        float GetAzimuthalAngle (void) const;
        
        float GetZenithAngle (void) const;

        void Update(long time);

        AdcsState GetState();
};

#endif
