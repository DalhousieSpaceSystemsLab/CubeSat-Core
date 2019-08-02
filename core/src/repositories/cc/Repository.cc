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

bool Repository::WatchListContainsKey(unsigned int key){

	//TODO there is the potential for optimizing this search by
	//sorting the array and using a search algorithm
	for(int i=0;i<this->watch_list_.size();i++){
		if(watch_list_.at(i)==key){
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
		if(WatchListContainsKey(float_keys[i])){

		}
	}

	for(int i=0;i<int_keys.size();i++){

	}


}
