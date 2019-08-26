#include "ProcessFilePaths.h"
#include "PowerRepository.h"
#include "CommsRepository.h"
#include "AdcsRepository.h"
#include "GpsRepository.h"
#include "PayloadRepository.h"
#include<stdlib.h>
#include<iostream>

ProcessFilePaths file_paths;

int StartPowerRepository();
int StartGpsRepository();
int StartCommsRepository();
int StartAdcsRepository();
int StartPayloadRepository();

int StartPowerRepository(){
	cout << "Starting Power Repository" << endl;
	/*
	 * Start power repository...
	 */
    PowerRepository power_server_(file_paths.power_repository);
    power_server_.WaitForConnection();

	return 1;
}

//TODO Put return codes to indicate success/fail into a struct?
int StartGpsRepository(){
	cout << "Starting Gps Repository" << endl;
	/*
	 * Start gps repository...
	 */
	GpsRepository gps_server_(file_paths.gps_repository);
	gps_server_.WaitForConnection();

	return 1;
}

int StartCommsRepository(){
	cout << "Starting Comms Repository" << endl;
	/*
	 * Start Comms repository...
	 */
	CommsRepository comms_server_(file_paths.comms_repository);
	comms_server_.WaitForConnection();
	return 1;
}

int StartPayloadRepository(){
	cout << "Starting Payload Repository" << endl;
	/*
	 * Start power repository...
	 */
	PayloadRepository payload_server_(file_paths.payload_repository);
	payload_server_.WaitForConnection();

	return 1;
}

int StartAdcsRepository(){
	cout << "Starting Adcs Repository" << endl;
	/*
	 * Start power repository...
	 */
	AdcsRepository adcs_server_(file_paths.adcs_repository);
	adcs_server_.WaitForConnection();
	return 1;
}

int TestRepositories(){
	//TODO have default constructor where the filepath is hard coded? But keep the current constructor optional
    PowerRepository power_server_(file_paths.power_repository);
     power_server_.WaitForConnection();

//    CommsRepository comms_server_(file_paths.comms_repository);
//	cout << file_paths.comms_repository << endl;
//	comms_server_.WaitForConnection();
//
////	AdcsRepository adcs_server_(file_paths.adcs_repository);
////	cout << file_paths.adcs_repository << endl;
////	adcs_server_.WaitForConnection();
////
////	PayloadRepository payload_server_(file_paths.payload_repository);
////	cout << file_paths.payload_repository << endl;
////	payload_server_.WaitForConnection();
////
////	GpsRepository gps_server_(file_paths.gps_repository);
////	cout << file_paths.gps_repository << endl;
////	gps_server_.WaitForConnection();

    return 1;
}

int main(int argc, char *argv[]){

	//argv[0] is the executable name
	//argv[1] is the option number

	if(argc>1){
		cout << "Starting process: " << argv[1] << endl;
		int option_number=atoi(argv[1]);

		switch(option_number){
			case 1 : StartPowerRepository(); break;
			case 2 : StartPayloadRepository(); break;
			case 3 : StartGpsRepository(); break;
			case 4 : StartCommsRepository(); break;
			case 5 : StartAdcsRepository(); break;
			default : cout << "Invalid argument" << endl; break;
			return 1;

		}
	}
	else{
		cout << "Insufficient arguments specified" << endl;
	    return 0;
	}
}