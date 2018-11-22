#include "../header/TerminalCommand.h"

TerminalCommand::TerminalCommand() {

}

TerminalCommand::TerminalCommand(std::string name, std::string description) {
    this->name_ = name;
    this->description_ = description;
}

std::string TerminalCommand::name() {
    return this->name_;
}

std::string TerminalCommand::description() {
    return this->description_;
}

std::string TerminalCommand::ToString() {
    return name_ + "," + description_;
}
