#include "../header/TerminalCommand.h"

TerminalCommand::TerminalCommand() {

}

TerminalCommand::TerminalCommand(std::string name, std::string description) {
    this->name_ = name;
    this->description_ = description;
}

//TODO refactor. Change getName to name
std::string TerminalCommand::getName() {
    return this->name_;
}

//TODO refactor. Change getDescription to description
std::string TerminalCommand::getDescription() {
    return this->description_;
}

//TODO refactor. Change toString to ToString
std::string TerminalCommand::toString() {
    return name_ + "," + description_;
}
