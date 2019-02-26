//  Battery.h/cc 
//      Basic implementation for the Battery class
//

#ifndef LORIS_BATTERIES_BATTERY_H_
#define LORIS_BATTERIES_BATTERY_H_

class Battery {
    public:
        Battery();

        float power_level() { return this->power_level_; }

        void set_power_level( float power_level ) { this->power_level_ = power_level ; }

    private:
        float power_level_;

};

#endif