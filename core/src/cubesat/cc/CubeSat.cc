#include "CubeSat.h"

// Null constructor for CubeSat class
//
CubeSat::CubeSat() {
    // Null
}

void CubeSat::SendMessageToGroundStation(const Message &message) {
    this->ground_station_.ProcessMessage(message);
}

void CubeSat::ProcessMessage(const Message &message) {
    std::string contents = message.getContents();

    if(contents=="GET_BATTERY"){
        std::string str=to_string(battery_manager_.avg_battery_level());
        Message *message = new Message(str);

        this->SendMessageToGroundStation(str);
    }
}
