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
	std::vector<int> string_keys = message.GetStringKeys();

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

	for(int i=0;i<string_keys.size();i++){
		unsigned int current_key = string_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,message.
					GetString(current_key));
		}
	}

	return 0;
}

int Repository::ReturnData(DataMessage request_message,DataMessage& return_message){
	std::vector<int> requests = request_message.GetRequests();
	int number_of_reqs=requests.size();
	if(number_of_reqs>0){
		cout << "Iterating through request keys" << endl;
		bool no_keys_watched_and_requested=true;
		//Iterate through all requested keys
		for(int i=0;i<number_of_reqs;i++){
			//Check if the requested key is watched by this repository
			unsigned int requested_key = requests[i];
			if(WatchListContainsKey(requested_key)){
				no_keys_watched_and_requested=false;
				//Check if the repository contains the requested key
				if(repository_data_.ContainsIntKey(requested_key)){
					return_message.Add(requested_key,
						repository_data_.GetInt(requested_key));
				}
				if(repository_data_.ContainsFloatKey(requested_key)){
					return_message.Add(requested_key,
						repository_data_.GetFloat(requested_key));
				}

			}
		}
		if(no_keys_watched_and_requested){
			cout << "The message contained requests which the repository does not contain data for." << endl;
		}
	}
	else{
		cout << "The data message contained no requests." << endl;
	}
}
