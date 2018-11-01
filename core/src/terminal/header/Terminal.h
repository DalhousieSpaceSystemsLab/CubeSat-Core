#ifndef TERMINAL_H
#define TERMINAL_H


#include <iostream>
#include "TerminalCommandLoader.h"
#include "TerminalCommand.h"
#include "GroundStation.h"
using namespace std;

class Terminal {
public:

	void chat();

	void sendCommand(char command[]);

	void listCommands();

	void displayMessage(char message[]);

	void loadCommands();

	void set_ground_station(GroundStation ground_station);

	std::vector<TerminalCommand> commands_;
	char command_directory_[];
	TerminalCommandLoader command_loader_;
	GroundStation ground_station_;

};

#endif
