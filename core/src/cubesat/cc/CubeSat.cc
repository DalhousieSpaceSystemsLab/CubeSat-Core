#include "CubeSat.h"

// Null constructor for CubeSat class
//
CubeSat::CubeSat() {
    // Null
}

//TODO change argument to type message
void CubeSat::SendMessageToGroundStation(const std::string &contents) {
    Message* message = new Message(contents);

    this->ground_station_.ProcessMessage(*message);
}

void CubeSat::ProcessMessage(const Message &message) {
    std::string contents = message.getContents();

    if(contents=="GET_BATTERY"){
        std::string str=to_string(battery_manager_.avg_battery_level());
        //TODO create a message object and send that to groundstation
        this->SendMessageToGroundStation(str);
    }
}

void CubeSat::set_ground_station(GroundStation ground_station) {
    this->ground_station_ = ground_station;
}
