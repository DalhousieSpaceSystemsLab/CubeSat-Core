//
// Created by Spencer Axford on 2019-05-16.
//

#include "PowerRepository.h"

#include "PowerKeys.h"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {
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
	//TODO add in checks to determine if there are requests.
        cout << "Processing Message Object in PowerRepository" << endl;
        KeyValuePairContainer c = message.GetMessageContents();

        cout << "Recipient : Sender : Time Created" << endl;
	cout << message.GetRecipient() << " : " << message.GetSender() << " : " << message.GetTimeCreated() << endl;
        
        //cout << "message type flag: " << message.GetFlag() << endl;

        cout << "requests:" << endl;
        std::vector<int> requests = message.GetRequests();
        for(int i = 0; i < requests.size(); i++){
                cout << requests.at(i) << endl;
        }
       
        cout << "Getting message key value pairs" << endl;

        std::vector<int> floatKeys = c.GetFloatKeys();
        std::vector<int> intKeys = c.GetIntKeys();
        std::vector<int> stringKeys = c.GetStringKeys();
        cout << "key value pairs:" << endl;
        for(int i = 0; i < intKeys.size(); i++){
                cout << intKeys.at(i) << " : " << c.GetInt(intKeys.at(i)) << endl; 
        }
        for(int i = 0; i < floatKeys.size(); i++){
                cout << floatKeys.at(i) << " : " << c.GetFloat(floatKeys.at(i)) << endl; 
        }
        for(int i = 0; i < stringKeys.size(); i++){
                cout << stringKeys.at(i) << " : " << c.GetString(stringKeys.at(i)) << endl; 
        }

        cout << "Adding data" << endl;
        AddData(message);

        return 0;
}
