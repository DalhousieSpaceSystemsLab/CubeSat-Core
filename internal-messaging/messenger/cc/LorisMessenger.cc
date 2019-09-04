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

int LorisMessenger::Add(unsigned int key, std::string value){
	this->current_message_.Add(key,value);
	return 0;
}

int LorisMessenger::AddRequest(int request){
	this->current_message_.AddRequest(request);
	return 0;
}

int LorisMessenger::ClearMessage(){
	//TODO potential memory leak...
	this->current_message_=DataMessage();
	return 0;
}

string LorisMessenger::Send(unsigned int recipient, unsigned int sender){
    MessageSendingService ms(recipient);
    this->current_message_.SetRecipient(recipient);
    this->current_message_.SetSender(sender);

    string reply=ms.SendDataMessage(this->current_message_);
    ClearMessage();
    return reply;
}
