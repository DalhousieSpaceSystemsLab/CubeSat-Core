#include "AdcsEmulator.h"
#include <iostream>

void AdcsEmulator::set_orientation(float azimuthal_angle, float zenith_angle)
{
    attitude_sensor_.set_azimuthal_angle(azimuthal_angle);
    attitude_sensor_.set_zenith_angle(zenith_angle);
}

float AdcsEmulator::GetAzimuthalAngle (void) const
{
   return attitude_sensor_.azimuthal_angle();
}

float AdcsEmulator::GetZenithAngle (void) const
{
    return attitude_sensor_.zenith_angle();
}


