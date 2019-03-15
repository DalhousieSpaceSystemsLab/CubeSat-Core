#include <iostream>
#include "StructSerializer.h"
#include <string>

//FOR TESTING

int main (void)
{
	
	cout << "------Test1------" << endl;
	
	Message* message = new Message;
	Message* answer = new Message;
	
	message->type = "Type Test 1";
	message->contents = "Contents Test 1";
	
	char data[PacketSize(message)];
	
	cout <<"message->type: " << message->type << ", message->contents: " << message->contents << endl;
	
	Serialize(message,data);
	Deserialize(answer,data);
	
	cout << "answer->type: " << answer->type << ",  answer->contents: " << answer->contents << endl;
	cout << "packetsize of message: " << PacketSize(message) << ", and answer: " << PacketSize(answer) << endl;
	
	delete message,answer;
	
	cout << "------Test2------" << endl;
	
	message = new Message;
	answer = new Message;
	char data_a[PacketSize(message)];
	
	message->type = "Different Size Type Test 2";
	message->contents = "Different Size Contents Test 2";
	
	cout <<"message->type: " << message->type << ", message->contents: " << message->contents << endl;
	
	Serialize(message,data_a);
	Deserialize(answer,data_a);
	
	cout << "answer->type: " << answer->type << ",  answer->contents: " << answer->contents << endl;
	cout << "packetsize of message: " << PacketSize(message) << ", and answer: " << PacketSize(answer) << endl;
	
	delete message,answer;
	
	cout << "------Test3------" << endl;
	
	message = new Message;
	answer = new Message;
	
	char data_b[PacketSize(message)];
	message->type = "3rd Small Test to Gaurentee deletee from 'message'";
	message->contents = "Contents round 3";
	
	cout <<"message->type: " << message->type << ", message->contents: " << message->contents << endl;
	
	Serialize(message,data_b);
	Deserialize(answer,data_b);
	
	cout << "answer->type: " << answer->type << ",  answer->contents: " << answer->contents << endl;
	cout << "packetsize of message: " << PacketSize(message) << ", and answer: " << PacketSize(answer) << endl;
	
	return 0;
}

