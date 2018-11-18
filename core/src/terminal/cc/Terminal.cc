#include "../header/Terminal.h";

void Terminal::chat() {
    int option;
    while (true) {
        cout << "What would you like to do?" << endl;
        cout << "(1) List commands" << endl;
        cout << "(2) Send command" << endl;
        cout << "(3) Exit" << endl;
        cin >> option;

        if (option == 1) {
            this->listCommands();
        } else if (option == 2) {
            string command;
            cout << "Enter desired command" << endl;
            cin >> command;
            this->sendCommand(command);
        } else if (option == 3) {
            break;
        } else {
            cout << "Please enter a valid option" << endl;
        }
    }
}

//TODO refactor. Capitalize first letter
void Terminal::sendCommand(string command) {
    Message* message = new Message(command);
    ground_station_->SendMessageToCubesat(command);
}

//TODO refactor. Capitalize first letter
void Terminal::listCommands() {
    if (commands_.size()) {
        for(int i=0; i < commands_.size(); i++) {
            cout << "Command #" << i << endl;
            cout << commands_[i].getName() << endl;
            cout << commands_[i].getDescription() << endl;
        }
    } else {
        cout << "No commands found" << endl;
    }
}
//TODO refactor. Capitalize first letter
void Terminal::displayMessage(Message &message) {
    cout << message.getContents() << endl;
}
//TODO refactor. Capitalize first letter
//TODO Change argument to string 
void Terminal::loadCommands(char *commandPath) {
    commands_ = command_loader_.readCommands(commandPath);
}

void Terminal::set_ground_station(GroundStation ground_station) {
    this->ground_station_ = ground_station;
}
