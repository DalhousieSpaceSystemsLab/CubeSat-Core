#include "Relay.h"
//#include "../header/Relay.h"

// Null constructor for Battery class
Relay::Relay() {
    // Null
}

bool Relay::toggle() {
    this->toggled_ = !this->toggled_;
    return this->toggled_;
}