//
// Created by John Barry on 2019-07-30.
//

#include "GpsRepository.h"

GpsRepository::GpsRepository(std::string filePaths)
        : Repository(filePaths) {}

//Adds all keys, to watch_list, for storing/returning future data
int GpsRepository::AddKeysToWatchList(){


	/* * Add keys here * */


	cout << "Printing watch list..." << endl;
	for(int i=0;i<watch_list_.size();i++){
		cout << i << watch_list_[i].key() << endl;
	}

	return 0;
}


int GpsRepository::ProcessMessage(DataMessage message){

    return 0;
}

unsigned int GpsRepository::repository_identifier(){
	Identifiers ids;
	return ids.gps_repository;
}

