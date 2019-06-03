#include "HardwareEmulationServer.h"
#include "SubsystemEmulator.h"
#include "PowerSerializer.h"
#include "PowerState.h"
#include "PowerServer.h"

HardwareEmulationServer::HardwareEmulationServer() {
    // null constructor
}

HardwareEmulationServer::HardwareEmulationServer(std::vector<SubsystemEmulator> emulators) {
    this->emulators_ = emulators;
}

void HardwareEmulationServer::Run() {
    while (true) {
        usleep(this->current_time_ * 1000);
        this->UpdateEmulators();
    }
}

void HardwareEmulationServer::UpdateEmulators() {
    for (int i=0; i < this->emulators_.size(); i++) {
        this->emulators_[i].Update(this->current_time_);
    }
}

void HardwareEmulationServer::SendDataToServer (SubsystemStruct *data, int port_number, int new_socket_file_descriptor) {
	// so far only communicates with power sever
	//TODO: needs higher level serrialization or more specific serializers for each emulation struct
	PowerServer power_server_(port_number);
	PowerState Pwr = new PowerState;
	Pwr = data;
	
	float* memory[4];
	Serialize(Pwr,memory);
	
	power_server_.HandleRequest(memory,new_socket_file_descriptor);
}
