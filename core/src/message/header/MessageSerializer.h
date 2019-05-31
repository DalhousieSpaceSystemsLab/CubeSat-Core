#ifndef MESSAGE_SERIALIZER
#define MESSAGE_SERIALIZER

#include "MessageHeader.h"
#include "Message.h"
#include "KeyValuePairContainer.h"
#include <iostream>

void SerializeMessage(Message* message, char *data)
{
	MessageHeader header_ = message->GetHeader();
	KeyValuePairContainer container_ = message->GetMessageContents();
	
    float *q = (float*)data;
    int i = 0;
    int n = 0;

	*q = (float)header_.GetSender();		q++;
    *q = (float)header_.GetRecipient();		q++;
    *q = (float)header_.GetTimeCreated();	q++;
    
    std::vector<int> keys = container_.GetKeys();
    
    while (i < keys.size()/2) {
    	*q = (float)keys[i];				q++;
    	*q = container_.GetFloat(i);		q++;
        i++;
	}
	
	*q = NULL; q++;

	while (i < keys.size()) {
		*q = (float)keys[i];				q++;
		*q = (float)container_.GetInt(n);	q++;
        i++;
        n++;
	}
	
	*q = NULL; q++;
}

Message DeserializeMessage(char *data)
{
    float *q = (float*)data;
    unsigned int sender_ = (unsigned int) *q;   q++;
    unsigned int reciever_ = (unsigned int) *q; q++;
    long time_sent_ = (long) *q;                q++;
    
    MessageHeader header_(reciever_,sender_,time_sent_);
     
    KeyValuePairContainer container_;
    
    while (*q != NULL) {
    	container_.AddKeyValuePair((unsigned int)*q, *(q+1));
    	q = q+2;
	}
	
	q++;
	
	while (*q != NULL) {
		container_.AddKeyValuePair((unsigned int)*q, (int)*(q+1));
		q = q+2;
	}
    
    Message de_message(header_, container_);
    
    return de_message;
}
#endif

