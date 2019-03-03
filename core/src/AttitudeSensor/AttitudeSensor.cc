#include "AttitudeSensor.h"

void AttitudeSensor::set_azimuthal_angle (float azimuthal_angle)
{
    this->azimuthal_angle_ = azimuthal_angle;
}

void AttitudeSensor::set_zenith_angle (float zenith_angle)
{
    this->zenith_angle_ = zenith_angle;
}

float AttitudeSensor::azimuthal_angle (void) const
{
    return azimuthal_angle_;
}

float AttitudeSensor::zenith_angle (void) const
{
    return zenith_angle_;
}