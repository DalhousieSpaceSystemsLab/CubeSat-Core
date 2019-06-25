#include "AdcsEmulator.h"

AdcsEmulator::AdcsEmulator(){
	set_orientation(0,0);
	this->initial_azimuth=0;
	this->initial_zenith=0;
}

AdcsEmulator::AdcsEmulator(float azimuthal_angle,float zenith_angle){
	set_orientation(azimuthal_angle,zenith_angle);
	this->initial_azimuth=azimuthal_angle;
	this->initial_zenith=zenith_angle;
}

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

void AdcsEmulator::GetState(Message & message)
{
	//TODO put the appropriate key value pairs into the message
}

//Time is in milliseconds
void AdcsEmulator::Update(long time)
{
	float ftime =time/1000;
	float const zenith_angular_velocity = 2;//deg/s
	float const azimuthal_angular_velocity = 5;//deg/s

    float new_azimuthal_angle=10*cos(azimuthal_angular_velocity*ftime)+
    		this->initial_azimuth;
    //current_zenith_angle+=zenith_angular_velocity*ftime;
    float new_zenith_angle=10*cos(zenith_angular_velocity*ftime)+
    		this->initial_zenith;

    this->set_orientation(new_azimuthal_angle,new_zenith_angle);
}


