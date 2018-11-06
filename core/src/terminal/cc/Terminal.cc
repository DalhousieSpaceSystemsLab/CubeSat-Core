#include "../header/Terminal.h";

void Terminal::chat() {
    //TODO Create a while loop
    //TODO In the while loop, display a list of options: 1. List Commands 2. Send command 3. Exit
    //TODO Have if statements to handle user input and call the appropriate functions
    
}

void Terminal::sendCommand(char *command) {
    //TODO Call this terminal's groundstation's "SendMessageToCubeSat" function. Pass the command argument to that function.
}

void Terminal::listCommands() {
    //TODO Get the commands loaded in _commands and print them
}

void Terminal::displayMessage(char *message) {
    //TODO Print message
}

void Terminal::loadCommands() {
    //TODO Call TerminalCommandLoader::readCommands
    
    //TODO Set commands_ to the returned array 
}

void Terminal::set_ground_station(GroundStation ground_station) {
    this->ground_station_ = ground_station;
}
