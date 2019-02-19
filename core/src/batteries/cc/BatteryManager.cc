#include "BatteryManager.h"
#include<iostream>
using namespace std;

// Null constructor for BatteryManager class
//
BatteryManager::BatteryManager() {

cout << "Initializing battery manager..." << endl;    

    for(int i=0 ; i<2 ; i++) {
        this->batteries_.push_back(new Battery());
        batteries_[i]->set_power_level(0.43);
    }
    for(int i=2 ; i<4 ; i++) {
        this->batteries_.push_back(new Battery());
        batteries_[i]->set_power_level(0.76);
    }
    cout << "Battery levels: " << endl;
    float batt_lvl;
    for(int i=0;i<4;i++){
	batt_lvl=this->batteries_[i]->power_level();
        cout << "Battery " << i << ":" << batt_lvl << endl;
    }
    cout << "Battery manager initialized." << endl;    
}

// Constructor for BatteryManager class
BatteryManager::BatteryManager(std::vector<Battery> batteries) {
    // Null
}

//
void BatteryManager::CalcAvgBatteryLevel() {

    int size;
    float sum=0;
    float batt_lvl=0;

	cout << "Calculating average battery level..." << endl;

    size = this->batteries_.size();
    for(int i=0 ; i<size ; i++) { 
	batt_lvl=this->batteries_[i]->power_level();
	cout << "Battery level" << i << ": "<< batt_lvl << endl;
        sum += batt_lvl; 
    }

    this->avg_battery_level_=(sum/size);

}
