#include "ProcessFilePaths.h"
#include "PowerRepository.h"
#include "CommsRepository.h"
#include "AdcsRepository.h"
#include "GpsRepository.h"
#include "PayloadRepository.h"

int main(){
    ProcessFilePaths file_paths;
    PowerRepository power_server_(file_paths.power_repository);
    cout << file_paths.power_repository << endl;
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
    return 0;
}
