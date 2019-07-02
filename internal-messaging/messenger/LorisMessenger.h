/*
 * LorisMessenger.h
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */

#include "Message.h"
#include "MessageSenderInterface.h"

#ifndef MESSENGER_LORISMESSENGER_H_
#define MESSENGER_LORISMESSENGER_H_
class LorisMessenger{
public:
	LorisMessenger();
	int Add(unsigned int key, int value);
	int Add(unsigned int key, float value);
	int Send(unsigned int recipient, unsigned int sender);
	int ClearMessage();

private:
	Message current_message_;

};
#endif /* MESSENGER_LORISMESSENGER_H_ */
