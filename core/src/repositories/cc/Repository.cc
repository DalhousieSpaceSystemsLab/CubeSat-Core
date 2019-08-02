//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {}

int Repository::HandleMessage(char *buffer){
    cout << "Handling message: " << buffer << endl;
    DataMessage msg = DataMessage(buffer);
    ProcessMessage(msg);
    return 0;
}

//Check if the key is contained in the watch list for the repository.
bool Repository::WatchListContainsKey(unsigned int key){

	//TODO there is the potential for optimizing this search by
	//sorting the array and using a search algorithm
	for(int i=0;i<this->watch_list_.size();i++){
		if(watch_list_.at(i).key()==key){
			return true;
		}
	}
	return false;
}

//Checks for key value pairs in message which have a key in the watch list
int Repository::AddData(DataMessage message){
	std::vector<int> float_keys = message.GetFloatKeys();
	std::vector<int> int_keys = message.GetIntKeys();

	for(int i=0;i<float_keys.size();i++){
		unsigned int current_key = float_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,message.
					GetFloat(current_key));
		}
	}

	for(int i=0;i<int_keys.size();i++){
		unsigned int current_key = int_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,message.
					GetInt(current_key));
		}
	}

	return 0;
}
