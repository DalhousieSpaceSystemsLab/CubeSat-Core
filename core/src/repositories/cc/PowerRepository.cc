//
// Created by Spencer Axford on 2019-05-16.
//

#include "PowerRepository.h"

#include "PowerKeys.h"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {
	this->identifier_=identifiers_.power_repository;
	AddKeysToWatchList();
}

//Adds all keys, to watch_list, for storing/returning future data
int PowerRepository::AddKeysToWatchList(){

	PowerKeys keys;
	this->watch_list_.push_back(keys.current_sensor1);
	this->watch_list_.push_back(keys.battery_level);

	cout << "Printing watch list..." << endl;
	for(int i=0;i<watch_list_.size();i++){
		cout << i << watch_list_[i].key() << endl;
	}

	return 0;
}

int PowerRepository::ProcessMessage(DataMessage message){

    cout << "Processing Message Object in PowerRepository" << endl;
    KeyValuePairContainer c = message.GetMessageContents();

    cout << "Recipient : Sender : Time Created" << endl;
	cout << message.GetRecipient() << " : " << message.GetSender() << " : " << message.GetTimeCreated() << endl;

    cout << "requests:" << endl;

    return 0;
}
