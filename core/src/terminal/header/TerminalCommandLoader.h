#ifndef TERMINAL_COMMAND_LOADER_H
#define TERMINAL_COMMAND_LOADER_H

#include <iostream>
#include <fstream>
#include "TerminalCommand.h"
#include <dirent.h>
#include <vector>
#include "string.h"
#include <string>
#include <sstream>

using namespace std;

class TerminalCommandLoader {
	public:
		std::vector<TerminalCommand> readCommands(char commandPath[]);

		TerminalCommand createCommand(const std::string& commandText);
};
#endif
