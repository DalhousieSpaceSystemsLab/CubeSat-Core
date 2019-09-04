/*
 * LorisMessenger.h
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */
#ifndef MESSENGER_LORISMESSENGER_H_
#define MESSENGER_LORISMESSENGER_H_

#include "DataMessage.h"
#include "MessageSendingService.h"
#include <string>

class LorisMessenger{
public:
	//Constructor
	LorisMessenger();
	
	//Methods to add key Value Pairs to message
	int Add(unsigned int key, int value);
	int Add(unsigned int key, float value);
	int Add(unsigned int key, std::string value);
	
	//Method to add a request for information to the message
	int AddRequest(int request);

	//Method to send stored message to recipient from sender (Using Identifer int - see Identifier.h for correct values)
	//TODO implement time out in UnixDomainStreamSocketClient for when server doesn't respond with reply
	string Send(unsigned int recipient, unsigned int sender);

	
	//Removes all information stored in message
	int ClearMessage();

private:
	DataMessage current_message_;

};
#endif /* MESSENGER_LORISMESSENGER_H_ */
