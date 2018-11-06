#include "GroundStation.h"

// Null constructor for GroundStation class
//
GroundStation::GroundStation() {
    // Null
}

// Constructor for GroundStation class
//
GroundStation::GroundStation(Terminal terminal) {
    this->terminal_=terminal;
}

//TODO Implement ProcessMessage(Message). 
//TODO Extract contents of the message 
//TODO Call terminal_.DisplayMessage and pass the message contents as the argument
