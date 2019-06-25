#include "HardwareEmulationManager.h"

#include "PowerEmulator.h"
#include "AdcsEmulator.h"
//#include "UnixDomainStreamSocketClient.h"
//#include "MessageSerializer.h"
//#include "MessageBuilder.h"

#include <iostream>
#include <sys/time.h>

/*
* Creates default emulator setup with pre-defined hardware states. 
*/
HardwareEmulationManager::HardwareEmulationManager() {
	this->true_time_between_updates_=1*1000;
	struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    
    this->current_time_ = ms;


    PowerEmulator eps_emulator;
    AdcsEmulator adcs_emulator;

    this->emulators_.push_back(&eps_emulator);
    this->emulators_.push_back(&adcs_emulator);

}

/*
* Creates emulation manager with custom emulators
*/

//HardwareEmulationManager::HardwareEmulationManager(std::vector<SubsystemEmulator *> emulators) {
   //TODO complete this
   // this->emulators_ = emulators;
//}

void HardwareEmulationManager::Run() {
    cout << "Updating emulators..." << endl;
    this->UpdateEmulators();
    this->current_time_+=this->emulation_time_increment_;
}

void HardwareEmulationManager::UpdateEmulators() {
	cout << "Hello1" << endl;
	cout << "size: "<< this->emulators_.size() << endl;
	cout << "Time: "<<this->current_time_<<endl;
    for (unsigned int i=0; i < this->emulators_.size(); i++) {
    	cout << i << endl;
    	this->emulators_[i]->Update(this->current_time_);
    }
    cout << "Hello" << endl;
}

void HardwareEmulationManager::SendDataToCore(Message data) {

}
