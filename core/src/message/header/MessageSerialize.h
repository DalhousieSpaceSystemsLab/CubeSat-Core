#ifndef MESSAGE_SERIALIZER
#define MESSAGE_SERIALIZER

#include "MessageHeader.h"
#include "Message.h"
#include "KeyValuePairContainer.h"
#include <typeinfo>
#include <iostream>

void message_serialize(Message* message, float *data)
{
    float *q = data;
    int i = 0;
    *q = (float) message->header_.sender;        q++;
    *q = (float) message->header_.recipient_;    q++;
    *q = (float) message->header_.time_created_; q++;
    
    std::vector<int> keys = message->contents_.GetKeys();
    
    //this loop checks type of data in keys in order to seperate float key pairs and int key pairs
    while (typeid(keys[i]).name() != 'i') {
    	*q = (float)keys[i];                          q++;
    	*q = message->contents_.GetFloat(i);          q++;
    	i++;
	}
	
	*q = NULL; q++;
	
	while (keys[i] != NULL) {
		*q = (float)keys[i];                          q++;
		*q = (float) message->contents_.GetInt(i);    q++;
	}
	
	*q = NULL; q++;
}

Message message_deserialize(float *data)
{
    float *q = data;
    unsigned int sender_ = (unsigned int) *q;   q++;
    unsigned int reciever_ = (unsigned int) *q; q++;
    long time_sent_ = (long) *q;                q++;
    
    MessageHeader h(reciever_,sender,time_sent);
     
    KeyValuePairContainer container;
    
    while (*q != NULL) {
    	container.AddKeyValuePair((unsigned int)*q, *(q+1));
    	q = q+2;
	}
	q++;
	while (*q != NULL) {
		container.AddKeyValuePair((unsigned int)*q, (int)*(q+1));
		q = q+2;
	}
    
    Message de_message(h, container);
    
    return de_message;
}
#endif

