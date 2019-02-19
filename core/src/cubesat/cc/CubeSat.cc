#include "CubeSat.h"
#include "GroundStation.h"
// Null constructor for CubeSat class
//
CubeSat::CubeSat() {
    // Null
	battery_manager_;
}

void CubeSat::SendMessageToGroundStation(const Message &message) {
    this->ground_station_->ProcessMessage(message);
}

void CubeSat::ProcessMessage(const Message &message) {
    std::string contents = message.getContents();

    if(contents=="GET_BATTERY"){
        //std::string str=std::to_string(battery_manager_.avg_battery_level());
	std::ostringstream ss;
	//Update average battery level
	float avg_batt_lvl = battery_manager_.avg_battery_level();
	ss << avg_batt_lvl;
	
	std::string str(ss.str());
	
	//std::string s = boost::lexical_cast<std::string>(avg_batt_lvl);
	Message *message = new Message(str);

        this->SendMessageToGroundStation(str);
    }
}
