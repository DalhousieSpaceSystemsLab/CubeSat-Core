//  Battery.h/cc 
//      Basic implementation for the Battery class
//

#ifndef LORIS_BATTERIES_BATTERY_H_
#define LORIS_BATTERIES_BATTERY_H_

class Battery {
    public:
        Battery();

        float getPowerLevel() { return this->power_level_; }

    private:
        float power_level_;

};

#endif