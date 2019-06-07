#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "HardwareEmulationServer.h"
#include "Relay.h"
#include "CommsEmulator.h"
#include "GroundStation.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    Battery battery1;
    Battery battery2;
    Relay relay;

    battery1.set_power_level(20);
    battery2.set_power_level(40.5);

    PowerEmulator emulator1(battery1);
    PowerEmulator emulator2;
    emulator2.set_main_battery(battery2);

    float power1 = 20;
    float power2 = 40.5;

    cout << "----- Begin Power Emulator testing -----" << endl << endl;

    if (power1 == emulator1.GetMainPower()) {
        cout << "emulator1 battery power correctly set" << endl;
    } else {
        cout << "error: emulator1 battery power" << endl;
    }

    if (power2 == emulator2.GetMainPower()) {
        cout << "emulator2 battery power correctly set" << endl;
    } else {
        cout << "error: emulator2 battery power" << endl;
    }

    bool turnOnRelay1 = emulator1.ToggleOBCRelay();
    if (turnOnRelay1) {
        cout << "relay1 successfully turned on" << endl;
    } else {
        cout << "error: emulator1 relay toggle" << endl;
    }

    emulator2.ToggleOBCRelay();
    bool turnOffRelay2 = emulator2.ToggleOBCRelay();
    if (!turnOffRelay2) {
        cout << "relay2 successfully turned off" << endl;
    } else {
        cout << "error: emulator1 relay toggle" << endl;
    }

    long interval_1 = 15;
    long interval_2 = 40;
    long interval_3 = 100;
    PowerEmulator emulator3(battery1);
    cout << "Current emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(interval_1);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(interval_2);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(interval_3);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(interval_3);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl << endl;

    cout << "----- Finished Power Emulator testing -----" << endl << endl;

    cout << "----- Begin Adcs Emulator testing -----" << endl << endl;

    AdcsEmulator adcsEmulator;
    adcsEmulator.set_orientation(10, 20);
    cout << "Current Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(interval_1);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(interval_2);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(interval_2);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(interval_3);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(interval_3);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl << endl;

    cout << "----- Finished Adcs Emulator testing -----" << endl << endl;

    cout << "----- Begin Comms Emulator testing -----" << endl << endl;

//    CommsEmulator commsEmulator;
    GroundStation groundStation;


    cout << "----- Begin Hardware Emulation Server testing -----" << endl << endl;

    std::vector<SubsystemEmulator> emulatorVector;
    emulatorVector.push_back(emulator1);
    emulatorVector.push_back(emulator3);
    emulatorVector.push_back(adcsEmulator);
    HardwareEmulationServer emulationServer1;
    HardwareEmulationServer emulationServer2(emulatorVector);
//    emulationServer2.Run();


    cout << "done" << endl;
    return 0;

}