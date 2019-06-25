#include <iostream>
#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "HardwareEmulationManager.h"
#include "Message.h"
#include <cstdio>
using std::cout;
using std::endl;


void TestPowerEmulator();
void TestAdcsEmulator();
void TestHardwareEmulationManager();
void TestGetStateString();
void MainTest();
void Emulate();

//TODO Choose function based on command line argument
int main() {
	//TestPowerEmulator();
	//TestAdcsEmulator();
	//TestHardwareEmulationManager();
	TestGetStateString();
	//Emulate();
}

void Emulate(){
	cout << "Creating HardwareEmulationManager..." << endl;
	HardwareEmulationManager manager;

	cout << "Running and calling all emulators updates." << endl;
	for(int i=0;i<10;i++){
		Message msg(0,0);
		manager.GetCurrentState(msg);

		manager.Run();
		cout << "sleeping..." << endl;
		usleep(10*1000000);
	}
}

void TestHardwareEmulationManager(){
	cout << "Creating HardwareEmulationManager..." << endl;
	HardwareEmulationManager manager;

	cout << "Running and calling all emulators updates." << endl;
	for(int i=0;i<10;i++){
		manager.Run();
		cout << "sleeping..." << endl;
		usleep(10*1000000);
	}
}

void TestPowerEmulator(){
  cout << "Creating PowerEmulator" << endl;
  PowerEmulator emulator;

  cout << "Emulating battery fluctuation." << endl;
  float power_level = emulator.GetMainPower();

  long fictional_time = 0;
  long delta_time = 10*1000;
  for(int i=0;i<10;i++){
	  float power_level = emulator.GetMainPower();
	  emulator.Update(fictional_time);
	  fictional_time+=delta_time;
	  printf("Power level at %dms: %f\n",fictional_time,power_level);
  }
}

void TestAdcsEmulator(){
	cout << "Creating AdcsEmulator" << endl;
	AdcsEmulator emulator;
	cout << "Emulating battery fluctuation." << endl;


	long fictional_time = 0;
	long delta_time = 10*1000;
	for(int i=0;i<10;i++){
		float zenith = emulator.GetZenithAngle();
		float azimuth = emulator.GetAzimuthalAngle();

		emulator.Update(fictional_time);

		fictional_time+=delta_time;
		printf("Attitude at %dms: Zenith, %f Azimuth, %f\n",fictional_time,+
				zenith,azimuth);
	}
}

void TestGpsEmulator(){

}

void TestGetStateString(){
	cout << "Creating HardwareEmulationManager..." << endl;
	HardwareEmulationManager manager;

	cout << "Calling GetState on all emulators..." << endl;
	int capacity=256;
	char hugeString[capacity];
	manager.GetCurrentStateString(hugeString,capacity);
	for(int i=0;i<capacity;i++){
		cout << hugeString[i];
	}

	cout << "Completed." << endl;
}

void MainTest(){
  //HardwareEmulationManager emulation_manager;
  //emulation_manager.Run(); 
}
