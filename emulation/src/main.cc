#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "HardwareEmulationServer.h"
#include "Relay.h"
#include "CommsEmulator.h"
#include "GroundStation.h"
#include <iostream>
#include <vector>

using namespace std;
#include HardwareEmulationServer.h


int main() {
  HardwareEmulationServer emulation_manager;
  emulation_manager.Run(); 

}