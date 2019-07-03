#include "ProcessFilePaths.h"
#include "PowerRepository.h"

int main(){
    ProcessFilePaths file_paths;
    PowerRepository power_server_(file_paths.power_repository);
    cout << file_paths.power_repository << endl;
    power_server_.WaitForConnection();
    return 0;
}
