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
 struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    
    this->current_time_ = ms;
    
    //Initialize batteries
    Battery battery;

    //Set initial power levels of batteries
    battery.set_power_level(40.5);

    PowerEmulator eps_emulator(battery);

    //Initialize ADCS Emulator
    AdcsEmulator adcs_emulator;
    adcs_emulator.set_orientation(10, 20);


    //this->emulators_.push_back(eps_emulator);
    //this->emulators_.push_back(adcs_emulator);
    this->emulators_.push_back(new PowerEmulator);  
}

/*
* Creates emulation manager with custom emulators
*/

//HardwareEmulationManager::HardwareEmulationManager(std::vector<SubsystemEmulator *> emulators) {
   //TODO complete this
   // this->emulators_ = emulators;
//}

void HardwareEmulationManager::Run() {
    while (true) {
        usleep(this->current_time_ * 1000);
        this->UpdateEmulators();
        cout << "Updating emulators..." << endl;
    }
}

void HardwareEmulationManager::UpdateEmulators() {
    for (int i=0; i < this->emulators_.size(); i++) {
        this->emulators_[i]->Update(this->current_time_);
    }
}

void HardwareEmulationManager::SendDataToCore(Message data) {

}
