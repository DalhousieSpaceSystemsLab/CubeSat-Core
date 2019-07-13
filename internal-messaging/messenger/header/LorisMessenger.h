/*
 * LorisMessenger.h
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */
#ifndef MESSENGER_LORISMESSENGER_H_
#define MESSENGER_LORISMESSENGER_H_

#include "Message.h"
#include "MessageSenderInterface.h"//TODO LorisMessenger include MessagingService. Make MessagingService implement the interface, not inherit from it.

class LorisMessenger{
public:
	//Constructor
	LorisMessenger();
	
	//Methods to add key Value Pairs to message
	int Add(unsigned int key, int value);
	int Add(unsigned int key, float value);
	
	//Method to send stored message to recipient from sender (Using Identifer int - see Identifier.h for correct values)
	int Send(unsigned int recipient, unsigned int sender);
	
	//Removes all information stored in message
	int ClearMessage();

private:
	Message current_message_;

};
#endif /* MESSENGER_LORISMESSENGER_H_ */
