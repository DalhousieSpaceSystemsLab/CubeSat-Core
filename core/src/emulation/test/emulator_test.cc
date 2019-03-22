#include "PowerEmulator.h"
#include "AdcsEmulator.h"
#include "Relay.h"
#include <iostream>

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

    PowerEmulator emulator3(battery1);
    cout << "Current emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(15);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(40);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(100);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl;
    emulator3.Update(100);
    cout << "New emulator3 battery power: " << emulator3.GetMainPower() << endl << endl;

    cout << "----- Finished Power Emulator testing -----" << endl << endl;

    cout << "----- Begin Adcs Emulator testing -----" << endl << endl;

    AdcsEmulator adcsEmulator;
    adcsEmulator.set_orientation(10, 20);
    cout << "Current Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(10);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(30);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(50);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(70);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl;
    adcsEmulator.Update(100);
    cout << "New Adcs Orientation: (" << adcsEmulator.GetAzimuthalAngle() << "," << adcsEmulator.GetZenithAngle() << ")" << endl << endl;

    cout << "----- Finished Adcs Emulator testing -----" << endl << endl;


    cout << "done" << endl;

}