#include "Relay.h"

Relay::Relay() {
    this->toggled_ = false;
}

bool Relay::toggle() {
    this->toggled_ = !this->toggled_;
    return this->toggled_;
}