//
// Created by John Barry on 2019-07-30.
//

#include "PayloadRepository.h"

PayloadRepository::PayloadRepository(unsigned int identifier)
        : Repository(identifier) {}

//Adds all keys, to watch_list, for storing/returning future data
int PayloadRepository::AddKeysToWatchList(){


	/* * Add keys here * */


	cout << "Printing watch list..." << endl;
	for(int i=0;i<watch_list_.size();i++){
		cout << i << watch_list_[i].key() << endl;
	}

	return 0;
}

int PayloadRepository::ProcessMessage(DataMessage message){

    return 0;
}

unsigned int PayloadRepository::repository_identifier(){
	Identifiers ids;
	return ids.payload_repository;
}

