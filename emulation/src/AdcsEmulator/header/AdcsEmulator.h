#ifndef ADCS_EMULATOR_H
#define ADCS_EMULATOR_H

#include "AttitudeSensor.h"
#include "SubsystemEmulator.h"
#include <iostream>
#include <math.h>
#include "DataMessage.h"
using namespace std;

class AdcsEmulator : public SubsystemEmulator
{
    private:

        AttitudeSensor attitude_sensor_;
        float initial_azimuth;
        float initial_zenith;

    public:

        AdcsEmulator();

        AdcsEmulator(float azimuthal_angle,float zenith_angle);

        void set_orientation (float azimuthal_angle, float zenith_angle);

        float GetAzimuthalAngle (void) const;
        
        float GetZenithAngle (void) const;

        void GetState(DataMessage & message);

        void Update(long time);
};

#endif
