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
	return this->current_message_.Add(key,value);
}

int LorisMessenger::Add(unsigned int key, float value){
	return this->current_message_.Add(key,value);
}

int LorisMessenger::ClearMessage(){
	this->current_message_= new Message;
	return 0;
}

int LorisMessenger::Send(unsigned int sender, unsigned int recipient){
    MessageSenderInterface ms(recipient);//TODO This should just be an interface...
    this->current_message_.SetRecipient(recipient);
    this->current_message_.SetSender(sender);

    ms.SendMessage(this->current_message_);
    ClearMessage();
    return 0;
}
