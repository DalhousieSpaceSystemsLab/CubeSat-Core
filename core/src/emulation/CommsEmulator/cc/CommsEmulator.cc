#include "CommsEmulator.h"

CommsEmulator::CommsEmulator():
    TCPServer(this->port_number_)
{
    // null
}