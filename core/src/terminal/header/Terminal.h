#ifndef TERMINAL_H
#define TERMINAL_H


#include <iostream>
#include "TerminalCommandLoader.h"
#include "TerminalCommand.h"
#include "GroundStation.h"
#include "Message.h"
using namespace std;

class Terminal {
public:

	void chat();

	void SendCommand(string command);

	void ListCommands();

	void DisplayMessage(string message);

	void LoadCommands();

	void set_ground_station(GroundStation ground_station);

	std::vector<TerminalCommand> commands_;
	char command_directory_[];
	TerminalCommandLoader command_loader_;
	GroundStation ground_station_;

};

#endif
