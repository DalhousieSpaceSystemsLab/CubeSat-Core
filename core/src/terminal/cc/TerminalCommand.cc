#include "../header/TerminalCommand.h"

TerminalCommand::TerminalCommand() {

}

TerminalCommand::TerminalCommand(std::string name, std::string description) {
    this->name_ = name;
    this->description_ = description;
}

std::string TerminalCommand::getName() {
    return this->name_;
}

std::string TerminalCommand::getDescription() {
    return this->description_;
}

std::string TerminalCommand::toString() {
    return name_ + "," + description_;
}
