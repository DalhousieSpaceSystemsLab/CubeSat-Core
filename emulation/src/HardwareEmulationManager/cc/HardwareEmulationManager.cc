#include "HardwareEmulationManager.h"

#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "DataMessage.h"
#include <sys/time.h>

/*
* Creates default emulator setup with pre-defined hardware states. 
*/
HardwareEmulationManager::HardwareEmulationManager() {
	this->true_time_between_updates_=1*1000;
	struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    
    this->current_time_ = 0;//ms;


    this->emulators_.push_back(new PowerEmulator);
    this->emulators_.push_back(new AdcsEmulator);
    //TODO Add emulator for GPS


}

/*
* Creates emulation manager with custom emulators
*/

//HardwareEmulationManager::HardwareEmulationManager(std::vector<SubsystemEmulator *> emulators) {
   //TODO complete this
   // this->emulators_ = emulators;
//}

long HardwareEmulationManager::Run() {
    //cout << "Updating emulators..." << endl;
    this->UpdateEmulators();
    this->current_time_+=this->emulation_time_increment_;
    return this->current_time_;
}

void HardwareEmulationManager::UpdateEmulators() {
    for (unsigned int i=0; i < this->emulators_.size(); i++){
    	this->emulators_[i]->Update(this->current_time_);
    }
}

void HardwareEmulationManager::SendDataToCore(DataMessage data) {

}

void HardwareEmulationManager::GetCurrentStateString(char * string, int capacity){
	DataMessage msg(0,0);
	GetCurrentState(msg);
	msg.ToString(string,capacity);
}

void HardwareEmulationManager::GetCurrentState(DataMessage & message){
	for (unsigned int i=0; i < this->emulators_.size(); i++){
    	this->emulators_[i]->GetState(message);
    }
}


