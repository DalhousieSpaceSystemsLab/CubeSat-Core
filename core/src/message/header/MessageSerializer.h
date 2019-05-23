#ifndef MESSAGE_SERIALIZER
#define MESSAGE_SERIALIZER

#include "MessageHeader.h"
#include "Message.h"
#include "KeyValuePairContainer.h"
#include <iostream>

void SerializeMessage(Message* message, char *data)
{
    float *q = (float*)data;
    int i = 0;
    
    *q = (float) message->header_.sender_;        q++;
    *q = (float) message->header_.recipient_;    q++;
    *q = (float) message->header_.time_created_; q++;
    
    std::vector<int> keys = message->contents_.GetKeys();
    
    while (message->contents_.GetFloat(i) != NULL) {
    	*q = (float)keys[i];                          q++;
    	*q = message->contents_.GetFloat(i);          q++;
    	i++;
	}
	
	*q = NULL; q++;
	
	while (keys[i] != NULL) {
		*q = (float)keys[i];                          q++;
		*q = (float) message->contents_.GetInt(i);    q++;
		i++;
	}
	
	*q = NULL; q++;
}

Message DeserializeMessage(char *data)
{
    float *q = (float*)data;
    
    unsigned int sender_ = (unsigned int) *q;   q++;
    unsigned int reciever_ = (unsigned int) *q; q++;
    long time_sent_ = (long) *q;                q++;
    
    MessageHeader header(reciever_,sender_,time_sent_);
     
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
    
    Message de_message(header, container);
    
    return de_message;
}
#endif

