#ifndef TERMINAL_H
#define TERMINAL_H


#include <iostream>
#include "TerminalCommandLoader.h"
#include "TerminalCommand.h"
#include "GroundStation.h"
#include "RFMessage.h"
using namespace std;

class GroundStation;
class Terminal {
public:
	
	Terminal();	

	void chat();

	void SendCommand(string command);

	void ListCommands();

	void DisplayMessage(RFMessage message);

	void LoadCommands(string commandPath);

	void set_ground_station(GroundStation &ground_station);

	std::vector<TerminalCommand> commands_;
	char command_directory_[];
	TerminalCommandLoader command_loader_;
	GroundStation *ground_station_;

};

#endif
