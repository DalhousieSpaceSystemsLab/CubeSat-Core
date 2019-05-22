//
// Created by Spencer Axford on 2019-05-16.
//

#include "../header/PowerRepository.h"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {}

int PowerRepository::HandleMessage(char *buffer, int new_socket_file_descriptor){
    cout << "Handling message " << buffer << endl;
    return 0;
}
