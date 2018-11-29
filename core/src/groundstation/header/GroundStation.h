
#ifndef LORIS_GROUNDSTATION_GROUNDSTATION_H_
#define LORIS_GROUNDSTATION_GROUNDSTATION_H_

#include <iostream>

#include "Messenger.h"
#include "Terminal.h"
#include "CubeSat.h"

class CubeSat;
class Terminal;
class GroundStation { //: public Messenger {
	public:
		GroundStation();
		//GroundStation(Terminal &terminal);
		
		void set_terminal(Terminal &terminal){ this->terminal_=&terminal;}
		void SendMessageToTerminal(const Message message);
		void SendMessageToCubeSat(const Message message);
		void ProcessMessage(const Message &message);

		void set_cubesat(CubeSat &cubesat) { this->cubesat_=&cubesat; }

	private:
		Terminal *terminal_;
		CubeSat *cubesat_;
		
};

#endif
