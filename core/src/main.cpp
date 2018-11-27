#include "terminal/header/Terminal.h"
#include "groundstation/header/GroundStation.h"
#include "cubesat/header/CubeSat.h"


int main() {
    Terminal terminal_;

    terminal_.LoadCommands();

    GroundStation ground_station_;

    terminal_.set_ground_station(ground_station_);

    ground_station_.set_terminal(terminal_);

    CubeSat cubesat_;

    cubesat_.set_ground_station(ground_station_);

    terminal_.chat();
}

