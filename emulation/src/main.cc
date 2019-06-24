#include <iostream>
#include "PowerEmulator.h"
#include "Message.h"
#include <cstdio>
using std::cout;
using std::endl;


void TestPowerEmulator();
void MainTest();

int main() {
  TestPowerEmulator();
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

}

void TestGpsEmulator(){

}

void MainTest(){
  //HardwareEmulationManager emulation_manager;
  //emulation_manager.Run(); 
}
