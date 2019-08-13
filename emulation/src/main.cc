#include <iostream>
#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "HardwareEmulationManager.h"
#include "LorisMessenger.h"
#include "DataMessage.h"
#include "PowerKeys.h"
#include "Identifiers.h"
#include <cstdio>
#include <string>
using std::cout;
using std::endl;


void TestPowerEmulator();
void TestAdcsEmulator();
void TestHardwareEmulationManager();
void TestGetStateString();
void MainTest();
void Emulate();
void TestLorisMessenger();

const int emulation_sleep_time_=3;//seconds
//TODO Choose function based on command line argument
int main() {
	//TestPowerEmulator();
	//TestAdcsEmulator();
	//TestHardwareEmulationManager();
	//TestGetStateString();
	//Emulate();
	TestLorisMessenger();
}

void Emulate(){
	cout << "Creating HardwareEmulationManager..." << endl;
	HardwareEmulationManager manager;

	cout << "Running and calling all emulators updates." << endl;
	printf("Time\t Battery Level\t Zenith\t 	Azimuth\t\n");
	for(int i=0;i<10;i++){
		DataMessage msg(0,0);
		manager.GetCurrentState(msg);
		std::vector<int> int_keys = msg.GetIntKeys();
		std::vector<int> float_keys = msg.GetFloatKeys();
/*
		//printf("%d int_keys and %d float_keys\n",int_keys.size(),float_keys.size());
		//printf("Printing keys...\n");
		for(int i=0;i<int_keys.size();i++){
			int key = int_keys[i];
			printf("%d ",key);
		}

		for(int i=0;i<float_keys.size();i++){
			int key = float_keys[i];
			printf("%d ",key);
		}

		printf("\n");
*/
		long fictional_time = manager.Run();
		//printf("Printing data for time %d\n",fictional_time);
		printf("%dms\t ",fictional_time);
		for(int i=0;i<int_keys.size();i++){
			int value = msg.GetInt(int_keys[i]);
			printf("%d\t",value);
		}

		for(int i=0;i<float_keys.size();i++){
			int key = float_keys[i];
			float value = msg.GetFloat(key);
			printf("%f\t",value);
		}

		printf("\n");


		//cout << "sleeping..." << endl;
		usleep(emulation_sleep_time_*1000000);

		//TODO make a function in HardwareEmulationManager called "Send(Message & message)"


	}
}

void TestLorisMessenger(){
	//Initialize the messenger
	cout << "Initializing Messenger" << endl;
	LorisMessenger tester;

	cout << "Generating data to send" << endl;
	//Add your data (arguments are: identifier for data, data value)
	PowerKeys power_keys;//Identifier for data generated from EPS

	std::string string = "This is a String";

	tester.Add(power_keys.current_sensor1,0.2f);//reading from a current sensor
	tester.Add(power_keys.battery_level,1);//battery level
	tester.Add(power_keys.battery_level,string);//string test
	tester.AddRequest(1); //examples of adding requests
	tester.AddRequest(2);

	//Look up the identifier for the recipient (in this case, its a process known as the "power repository")
	Identifiers recipients;

	//The second argument here is the ID of whoever initiated sending the message.
	cout << "Sending data to recipient " << recipients.power_repository << endl;

	std::string reply="";

	reply = tester.Send(recipients.power_repository,recipients.power_subsystem);

	cout << "reply: " << reply << endl;
	DataMessage message(reply);

	cout << "test" << endl;

//	cout << "Sending data to recipient " << recipients.comms_repository << endl;
//	tester.Send(recipients.comms_repository,0);
//
//	cout << "Sending data to recipient " << recipients.adcs_repository << endl;
//	tester.Send(recipients.adcs_repository,0);
//
//	cout << "Sending data to recipient " << recipients.payload_repository << endl;
//	tester.Send(recipients.payload_repository,0);
//
//	cout << "Sending data to recipient " << recipients.gps_repository << endl;
//	tester.Send(recipients.gps_repository,0);
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
