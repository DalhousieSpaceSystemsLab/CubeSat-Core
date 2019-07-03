/*#include <iostream>
#include "ProcessFilePaths.h"
#include "PowerRepository.h"

using namespace std;

int main() {
    cout << "Starting program" << endl;

    ProcessFilePaths file_paths;
    cout << "Power socket stored at" << file_paths.power_repository;
    PowerRepository power_server_(file_paths.power_repository);
    power_server_.WaitForConnection();
    return 0;
}
*/
