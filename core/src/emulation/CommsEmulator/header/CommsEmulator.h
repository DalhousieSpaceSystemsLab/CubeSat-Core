
#ifndef LORIS_COMMSEMULATOR_COMMSEMULATOR_H_
#define LORIS_COMMSEMULATOR_COMMSEMULATOR_H_

#include <iostream>

#include "TCPServer.h"
#include "SubsystemEmulator.h"

class CommsEmulator: public SubsystemEmulator, public TCPServer {
public:
    CommsEmulator();

private:
    const static int port_number_ = 3001;

};

#endif
