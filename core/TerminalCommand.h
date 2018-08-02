#include <iostream>

using namespace std;

class TerminalCommand {
public:
	TerminalCommand() {

	}

	TerminalCommand(std::string name, std::string description) {
		this->name_ = name;
		this->description_ = description;
	}
	std::string getName() {
		return this->name_;
	}

	std::string getDescription() {
		return this->description_;
	}

	std::string toString() {
		return name_ + "," + description_;
	}
	
	std::string name_;
	std::string description_;
};