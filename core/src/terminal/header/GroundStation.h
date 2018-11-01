#include <iostream>
#include "Messenger.h"
#include "../cc/Terminal.cc";

using namespace std;

class GroundStation: public Messenger {
	
	void SendMessageToTerminal(char message[]);

	void set_terminal(Terminal terminal);

	Terminal terminal_;
};