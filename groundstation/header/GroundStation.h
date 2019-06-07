
#ifndef LORIS_GROUNDSTATION_GROUNDSTATION_H_
#define LORIS_GROUNDSTATION_GROUNDSTATION_H_

#include <iostream>

#include "Messenger.h"
#include "Terminal.h"

class Terminal;
class GroundStation { //: public Messenger {
	public:
		GroundStation();
		//GroundStation(Terminal &terminal);
		
		void set_terminal(Terminal &terminal){ this->terminal_=&terminal;}
		void SendMessageToTerminal(const RFMessage message);
		void SendMessageToCubeSat(const RFMessage message);
		void ProcessMessage(const RFMessage &message);

	private:
		Terminal *terminal_;
		const static int comms_port_number_ = 3001;

};

#endif
