//
// Created by Spencer Axford on 2019-05-16.
//

#include "../header/PowerRepository.h"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {}

int PowerRepository::ProcessMessage(Message message){
        cout << "Processing Message Object in PowerRepository";
        return 0;
}
