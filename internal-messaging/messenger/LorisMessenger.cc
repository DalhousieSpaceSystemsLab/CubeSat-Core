/*
 * LorisMessenger.cc
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */

#include "LorisMessenger.h"

LorisMessenger::LorisMessenger(){
	ClearMessage();
}

int LorisMessenger::Add(unsigned int key, int value){
	this->current_message_.Add(key,value);
	return 0;
}

int LorisMessenger::Add(unsigned int key, float value){
	this->current_message_.Add(key,value);
	return 0;
}

int LorisMessenger::ClearMessage(){
	//TODO potential memory leak...
	this->current_message_=Message();
	return 0;
}

int LorisMessenger::Send(unsigned int recipient, unsigned int sender){
    MessageSenderInterface ms(recipient);//TODO This should just be an interface...
    this->current_message_.SetRecipient(recipient);
    this->current_message_.SetSender(sender);

    ms.SendMessage(this->current_message_);
    ClearMessage();
    return 0;
}
