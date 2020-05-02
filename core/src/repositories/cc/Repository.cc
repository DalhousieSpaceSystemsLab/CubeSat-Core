//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(unsigned int identifier)
        : mrs(identifier) {}

int Repository::ExtractDataFromReceivedMessage(DataMessage received_message){
    //TODO remove the cout statements when deemed "too annoying and unnecessary"
	cout << "Extracting data from received message" << endl;
    KeyValuePairContainer data = received_message.GetMessageContents();

    cout << "Adding data" << endl;
    //Get all keys for each primitive type
	std::vector<int> float_keys = received_message.GetFloatKeys();
	std::vector<int> int_keys = received_message.GetIntKeys();
	std::vector<int> string_keys = received_message.GetStringKeys();

	//Iterate through all key float value pairs and add any that are watched
	for(int i=0;i<float_keys.size();i++){
		unsigned int current_key = float_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetFloat(current_key));
		}
	}

	//Iterate through all key int value pairs and add any that are watched
	for(int i=0;i<int_keys.size();i++){
		unsigned int current_key = int_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetInt(current_key));
		}
	}

	//Iterate through all key string value pairs and add any that are watched
	for(int i=0;i<string_keys.size();i++){
		unsigned int current_key = string_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetString(current_key));
		}
	}

	return 0;
}

int Repository::ReplyToConnectedClient(DataMessage& message){
	mrs.Reply(message);
	return 1;
}


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

int Repository::BuildReturnDataMessage(DataMessage request_message,DataMessage& return_message){
	cout << "Building return message" << endl;
	std::vector<int> requests = request_message.GetRequests();
	int number_of_reqs=requests.size();
	if(number_of_reqs>0){
		cout << "Iterating through request keys" << endl;
		bool no_keys_watched_and_requested=true;
		//Iterate through all requested keys
		for(int i=0;i<number_of_reqs;i++){
			//Check if the requested key is watched by this repository
			unsigned int requested_key = requests[i];
			cout << "uint requested_key " << requested_key << endl;
			cout << "requested_key " << requests[i] << endl;

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
	return 1;
}

void Repository::start() {
	Message *message;
	mrs.StartListeningForClients();
	while(true){
		mrs.ListenForMessage(message);
		if ( DataMessage * dm = dynamic_cast<DataMessage*>( message ) ) {
			cout << "Message was a DataMessage" << endl;
			//Extract any new, watched data from the message
			ExtractDataFromReceivedMessage(*dm);

			//Perform any additional, optional processing for the message
			ProcessMessage(*dm);

			//Build an empty reply message
			DataMessage reply_message(this->repository_identifier(), dm->GetSender());

			//Append any requested data to the reply_message, if the repository has it
			if(dm->HasRequests()){
				cout << "Message has requests for information" << endl;
				BuildReturnDataMessage(*dm,reply_message);
				
				//Reply to the client
				ReplyToConnectedClient(reply_message);
			}
		}
		else if ( CommandMessage * cm = dynamic_cast<CommandMessage*>( message ) ) {
			//TODO Add process for command messages
		}
		else {
			cout << "Unable to cast to a known message type" << endl;
			throw "unknown message type";
		}
	}
}