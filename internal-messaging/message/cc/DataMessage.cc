#include <iostream>
#include <cstring>
#include "DataMessage.h"
#include <stdexcept>

DataMessage::DataMessage(unsigned int sender, unsigned int recipient) :
		DataMessage(sender, recipient, 0, KeyValuePairContainer()) {
}

DataMessage::DataMessage(unsigned int sender, unsigned int recipient, long time,
		KeyValuePairContainer contents) :
		Message(sender, recipient, time, 100, contents) {
}

DataMessage::DataMessage(string flat) :
	DataMessage(flat, DEFAULT_MESSAGE_CAPACITY) {
}

DataMessage::DataMessage(char *flat) :
	DataMessage(flat, DEFAULT_MESSAGE_CAPACITY) {
}

DataMessage::DataMessage(string flat, unsigned int max_size) {
	setCapacity(max_size);
	char cstr[flat.size() + 1];
	std::strcpy(cstr, flat.c_str());	// or pass &s[0]
	BuildFromCharacters(cstr);
}

DataMessage::DataMessage(char *flat, unsigned int max_size) {
	setCapacity(max_size);
	BuildFromCharacters(flat);
}

int DataMessage::BuildFromCharacters(char *flat) {
	std::cout << "Creating Data Message" << std::endl;
	char integer_string[32];
	char hex_string[32];
	memset(integer_string, 0, 32);
	memset(hex_string, 0, 32);
	// Find sender, recipient, time and flag
	int i = BuildHeader(flat, 0);
	i++;

	//TODO Find more robust solution for when no requests exist
	// Find Requests
	while (flat[i] != *section_break) {
		std::cout << "Getting requests" << std::endl;
		// Get next request
		//TODO fix bug where infinite loop is reached due to no requests in the message -- I have not see this bug?
		while (flat[i] != '-' && flat[i] != *section_break) {
			sprintf(integer_string, "%c", flat[i]);
			strcat(hex_string, integer_string);
			i++;
		}
		try {
			std::cout << "Adding: " << hex_string
					<< " to requests in DataMessage" << std::endl;
			int request = std::stoi(hex_string, nullptr, 16);
			requests.push_back(request);
		} catch (std::exception const &e) {
		}
		memset(integer_string, 0, 32);
		memset(hex_string, 0, 32);
		if (flat[i] != *section_break) {
			i++;
		};
	}
	// Find key value pairs
	i++;
	i = BuildContents(flat, i);
	return 1;
}

void DataMessage::Flatten(char *msg) {
	int message_size = 0;
	char integer_string[32];

	// Add sender, recipient, time, and flag
	FlattenHeader(msg, message_size);
	if (flag != 100) {
		throw std::invalid_argument(
				"Flattened message had incorrect flag for a DataMessage");
	}

	// If there are no requests, only a | will be added
	if (requests.size() == 0) {
		message_size++;
		if (message_size > capacity) {
			throw std::invalid_argument("Message to large");
		}
	}

	// Add requests
	for (int i = 0; i < requests.size(); i++) {
		sprintf(integer_string, "%x", requests.at(i));
		message_size += strlen(integer_string) + 1;
		if (message_size > capacity) {
			throw std::invalid_argument("Message to large");
		}
		if (i == requests.size() - 1) {
			strcat(msg, integer_string);
		} else {
			strcat(msg, integer_string);
			strcat(msg, request_break);
		}
	}
	strcat(msg, section_break);
	// Add Key Value Pairs
	this->contents_.Flatten(msg, message_size, capacity);
	message_size += 1;
	if (message_size > capacity) {
		throw std::invalid_argument("Message to large");
	}
	// Add end char
	strcat(msg, message_break);
}

void DataMessage::AddRequest(int request) {
	requests.push_back(request);
}

bool DataMessage::HasRequests() {
	if (this->requests.size() > 0) {
		return true;
	}
	return false;
}

std::vector<int>& DataMessage::GetRequests() {
	std::vector<int> &request_copy = requests;
	return request_copy;
}
