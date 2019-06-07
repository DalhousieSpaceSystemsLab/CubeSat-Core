#ifndef TERMINAL_COMMAND_H
#define TERMINAL_COMMAND_H

#include <iostream>

//TODO remove using statement
using namespace std;

class TerminalCommand {
public:
	TerminalCommand();

	TerminalCommand(std::string name, std::string description);

	std::string name();

	std::string description();

	std::string ToString();
	
	std::string name_;
	std::string description_;
};
#endif
