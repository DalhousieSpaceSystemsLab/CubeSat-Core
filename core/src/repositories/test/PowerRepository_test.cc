//
// Created by Spencer Axford on 2019-05-16.
//

#include "../cc/PowerRepository.cc"
#include "../../message/identifiers/ProcessFilePaths.h"
int main(int argc, char *argv[])
{
    ProcessFilePaths file_paths;
    PowerRepository power_server_(file_paths.power_repository);
    power_server_.WaitForConnection();
    return 0;
}