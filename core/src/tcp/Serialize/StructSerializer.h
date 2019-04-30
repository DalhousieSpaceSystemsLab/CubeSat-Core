#ifndef STRUCT_SERIALIZER
#define STRUCT_SERIALIZER
#include <iostream>
#include "Message.h"
#include <string>

using namespace std;

//takes a message struct and a character array called data and stores all info one bit at a time in memory
void Serialize (Message* message, char* data)
{
	char* s = data;
	//at the loacation of data in memory, begin storing mssage->type and then itterate
	for(int i = 0; i < message->type.length(); i++)
	{
		*s = message->type.at(i);  s++;
	}
	
	*s = '\0';
	s++;
	// same as above for message->contents
	for (int i = 0; i < message->contents.length(); i++)
	{
		*s = message->contents.at(i);  s++;
	}
	*s = '\0';
}

// goes to data space in memory and beings taking each bit and pushing it into the deserialized message
void Deserialize (Message* message, char* data)
{
	char *d = data;
	//data moved to message->type and then itterated
	while(*d != '\0')
	{
		message->type.push_back(*d);  d++;
	}
	
	d++;
	// same as above but sent to message->content instead
	while (*d != '\0')
	{
		message->contents.push_back(*d);  d++;
	}
}

// used to find ideal packet size for data array, ie what the max size of struct is going to be
long PacketSize (Message* message)
{
	return (message->type.length() + message->contents.length() + 1);	
}

#endif


