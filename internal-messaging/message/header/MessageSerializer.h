#ifndef MESSAGE_SERIALIZER
#define MESSAGE_SERIALIZER

#include "Message.h"
#include "KeyValuePairContainer.h"
#include <iostream>

void SerializeMessage(Message* message, char *data)
{
	KeyValuePairContainer container_ = message->GetMessageContents();
	
    float *q = (float*)data;
    int i = 0;
    int n = 0;

	*q = (float)message->GetSender();		q++;
    *q = (float)message->GetRecipient();	q++;
    *q = (float)message->GetTimeCreated();	q++;
    
    std::vector<int> keys = container_.GetKeys();
    
    while (i < keys.size()/2) {
    	*q = (float)keys[i];				q++;
    	*q = container_.GetFloat(i);		q++;
        i++;
	}
	
	*q = 0.0; q++;

	while (i < keys.size()) {
		*q = (float)keys[i];				q++;
		*q = (float)container_.GetInt(n);	q++;
        i++;
        n++;
	}
	*q = 0.0; q++;
}

Message DeserializeMessage(char *data)
{
	std::cout << "deserializing " << data[0] << std::endl;

    float *q = (float*)data;
    unsigned int sender_ = (unsigned int) *q;   q++;
    unsigned int reciever_ = (unsigned int) *q; q++;
    long time_sent_ = (long) *q;                q++;
         
    KeyValuePairContainer container_;
    
    while (*q != 0.0) {
    	container_.AddKeyValuePair((unsigned int)*q, *(q+1));
    	q = q+2;
	}
	
	q++;
	
	while (*q != 0.0) {
		container_.AddKeyValuePair((unsigned int)*q, (int)*(q+1));
		q = q+2;
	}
    
    Message de_message(sender_, reciever_, time_sent_, container_);
    
    return de_message;
}
#endif

