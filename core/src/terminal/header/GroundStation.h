#include <iostream>
#include "Messenger.h"
#include "Terminal.h";

using namespace std;

class GroundStation: public Messenger {
	
	void SendMessageToTerminal(char message[]);

	void set_terminal(Terminal terminal);

	Terminal terminal_;
};