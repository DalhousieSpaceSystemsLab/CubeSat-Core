/*
 * LorisMessenger.h
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */
#ifndef MESSENGER_LORISMESSENGER_H_
#define MESSENGER_LORISMESSENGER_H_

#include "DataMessage.h"
#include "MessageSenderInterface.h"//TODO LorisMessenger include MessagingService. Make MessagingService implement the interface, not inherit from it.
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
	int Send(unsigned int recipient, unsigned int sender);

	//Method to send stored message to recipient from sender (Using Identifer int - see Identifier.h for correct values)
	//Awaits reply from recipient until message recieved or timeout TODO implement timeout in SendAndAwaitReply
	int SendAndAwaitReply(unsigned int recipient, unsigned int sender,string reply);


	
	//Removes all information stored in message
	int ClearMessage();

private:
	DataMessage current_message_;

};
#endif /* MESSENGER_LORISMESSENGER_H_ */
