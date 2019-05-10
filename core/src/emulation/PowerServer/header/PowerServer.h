#ifndef POWER_SERVER
#define POWER_SERVER

#include <iostream>
#include "../src/tcp/header/TCPServer.h"
#include "../emulation/HardwareEmulationServer/header/PowerSerializer.h"
#include "../emulation/HardwareEmulationServer/header/PowerState.h"

class PowerServer: public TCPServer {
	public:
		void HandleRequest(float* data, int new_socket_file_descriptor)
		{
			PowerState de_serial_pwr = new PowerState;
			Deserialize(data,de_serial_pwr);
			
			std::cout << "Main batterey Level: " << de_serial_pwr->main_battery_level << std::endl;
			std::cout << "OBC relay toggle: " << de_serial_pwr->obc_relay_toggled << std::endl;
		}
};

#endif
