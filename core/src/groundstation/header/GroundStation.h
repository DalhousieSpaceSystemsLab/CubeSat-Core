#ifndef LORIS_GROUNDSTATION_GROUNDSTATION_H_
#define LORIS_GROUNDSTATION_GROUNDSTATION_H_

#include <iostream>

#include "Messenger.h"
#include "Terminal.h"
#include "CubeSat.h"

class GroundStation: public Messenger {
	public:
		GroundStation();
		GroundStation(Terminal terminal);

		//TODO change argument to type message
		void SendMessageToTerminal(std::string message) { this->terminal_.displayMessage(message); }
		//TODO change Message() constructor call. Just pass message argument.
		void SendMessageToCubesat(Message &message) { this->cubesat_.ProcessMessage(Message(message)); }
		void ProcessMessage(const Message &message);

		void SetCubeSat(CubeSat cubesat) { this->cubesat_=cubesat; }

	private:
		Terminal terminal_;
		CubeSat cubesat_;
		
};

#endif
