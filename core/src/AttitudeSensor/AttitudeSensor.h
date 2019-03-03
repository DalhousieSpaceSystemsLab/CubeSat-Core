#ifndef ATTITUDE_SENSOR_H
#define ATTITTUDE_SENSOR_H

#include <iostream>

using namespace std;

class AttitudeSensor {
    private:

        double azimuthal_angle_;
        
        double zenith_angle_;
    public:

        void set_azimuthal_angle(float azimuthal_angle);

        void set_zenith_angle(float zenith_angle);

        float azimuthal_angle(void) const;

        float zenith_angle(void) const;
};

#endif