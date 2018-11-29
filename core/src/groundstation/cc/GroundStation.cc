#include "GroundStation.h"

// Null constructor for GroundStation class
//
GroundStation::GroundStation() {
    // Null
}

// Constructor for GroundStation class
//
//GroundStation::GroundStation(Terminal terminal) {
//    this->terminal_=terminal;
//}

void GroundStation::ProcessMessage(const Message &message) {
    //this->
	SendMessageToTerminal(message);
}

void GroundStation::SendMessageToTerminal(const Message message){ this->terminal_->DisplayMessage(message); }
void GroundStation::SendMessageToCubeSat(const Message message) { this->cubesat_->ProcessMessage(message); }
