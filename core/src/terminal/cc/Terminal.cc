#include "../header/Terminal.h";

Terminal(){
// null
}

void Terminal::chat() {
    int option;
    while (true) {
        cout << "What would you like to do?" << endl;
        cout << "(1) List commands" << endl;
        cout << "(2) Send command" << endl;
        cout << "(3) Exit" << endl;
        cin >> option;

        if (option == 1) {
            this->ListCommands();
        } else if (option == 2) {
            string command;
            cout << "Enter desired command" << endl;
            cin >> command;
            this->SendCommand(command);
        } else if (option == 3) {
            break;
        } else {
            cout << "Please enter a valid option" << endl;
        }
    }
}

void Terminal::SendCommand(string command) {
    Message* message = new Message(command);
    ground_station_->SendMessageToCubesat(command);
}

void Terminal::ListCommands() {
    if (commands_.size()) {
        for(int i=0; i < commands_.size(); i++) {
            cout << "Command #" << i << endl;
            cout << commands_[i].name() << endl;
            cout << commands_[i].description() << endl;
        }
    } else {
        cout << "No commands found" << endl;
    }
}
void Terminal::DisplayMessage(Message &message) {
    cout << message.getContents() << endl;
}
void Terminal::LoadCommands(string commandPath) {
    commands_ = command_loader_.ReadCommands(commandPath);
}

void Terminal::set_ground_station(GroundStation ground_station) {
    this->ground_station_ = ground_station;
}
