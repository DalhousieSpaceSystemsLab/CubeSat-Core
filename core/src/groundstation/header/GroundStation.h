
#ifndef LORIS_GROUNDSTATION_GROUNDSTATION_H_
#define LORIS_GROUNDSTATION_GROUNDSTATION_H_

class CubeSat;

#include <iostream>

#include "Messenger.h"
#include "Terminal.h"
#include "CubeSat.h"

class GroundStation: public Messenger {
	public:
		GroundStation();
		//GroundStation(Terminal &terminal);
		
		void set_terminal(Terminal &terminal){ this->terminal_=terminal;}
		void SendMessageToTerminal(const Message &message) { this->terminal_.DisplayMessage(message); }
		void SendMessageToCubesat(const Message &message) { this->cubesat_.ProcessMessage(message); }
		void ProcessMessage(const Message &message);

		void SetCubeSat(const CubeSat &cubesat) { this->cubesat_=cubesat; }

	private:
		Terminal terminal_;
		CubeSat cubesat_;
		
};

#endif
