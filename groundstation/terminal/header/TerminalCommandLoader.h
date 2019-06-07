#ifndef TERMINAL_COMMAND_LOADER_H
#define TERMINAL_COMMAND_LOADER_H

#include <iostream>
#include <fstream>
#include "TerminalCommand.h"
#include <dirent.h>
#include <vector>
#include "string.h"
#include <string>
#include <cstring>
#include <sstream>

//TODO remove using statement
using namespace std;

class TerminalCommandLoader {
	public:
		std::vector<TerminalCommand> ReadCommands(std::string commandPath);

		TerminalCommand CreateCommand(const std::string& commandText);
};
#endif
