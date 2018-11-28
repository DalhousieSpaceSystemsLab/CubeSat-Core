#include "Terminal.h"
#include "GroundStation.h"
#include "CubeSat.h"


int main() {
    Terminal terminal_;

    terminal_.LoadCommands();

    GroundStation ground_station_;

    ground_station_.set_terminal(terminal_);

    terminal_.set_ground_station(ground_station_);

    CubeSat cubesat_;

    cubesat_.set_ground_station(ground_station_);

    terminal_.chat();
}

