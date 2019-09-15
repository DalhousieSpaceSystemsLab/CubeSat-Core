//
// Created by John Barry on 2019-07-30.
//

#include "AdcsRepository.h"

AdcsRepository::AdcsRepository(unsigned int identifier)
        : Repository(identifier) {}

//Adds all keys, to watch_list, for storing/returning future data
int AdcsRepository::AddKeysToWatchList(){


	/* * Add keys here * */


	cout << "Printing watch list..." << endl;
	for(int i=0;i<watch_list_.size();i++){
		cout << i << watch_list_[i].key() << endl;
	}

	return 0;
}


int AdcsRepository::ProcessMessage(DataMessage message){

    return 0;
}


unsigned int AdcsRepository::repository_identifier(){
	Identifiers ids;
	return ids.adcs_repository;
}
