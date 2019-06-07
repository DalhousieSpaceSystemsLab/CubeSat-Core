#include "GroundStation.h"

// Null constructor for GroundStation class
//
GroundStation::GroundStation()
{
    // Null
}

// Constructor for GroundStation class
//
//GroundStation::GroundStation(Terminal terminal) {
//    this->terminal_=terminal;
//}

void GroundStation::ProcessMessage(const RFMessage &message) {
    //this->
	SendMessageToTerminal(message);
}

void GroundStation::SendMessageToTerminal(const RFMessage message){ this->terminal_->DisplayMessage(message); }
