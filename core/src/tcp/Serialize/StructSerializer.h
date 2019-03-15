#ifndef STRUCT_SERIALIZER
#define STRUCT_SERIALIZER
#include <iostream>
#include "Struct.h"
#include <string>

using namespace std;

void Serialize (Message* message, char* data)
{
	char* s = data;
	
	for(int i = 0; i < message->type.length(); i++)
	{
		*s = message->type.at(i);  s++;
	}
	
	*s = '\0';
	s++;
	
	for (int i = 0; i < message->contents.length(); i++)
	{
		*s = message->contents.at(i);  s++;
	}
	*s = '\0';
}

void Deserialize (Message* message, char* data)
{
	char *d = data;
	
	while(*d != '\0')
	{
		message->type.push_back(*d);  d++;
	}
	
	d++;
	
	while (*d != '\0')
	{
		message->contents.push_back(*d);  d++;
	}
}

long PacketSize (Message* message)
{
	return (message->type.length() + message->contents.length() + 1);	
}

#endif


