//  CubeSat.h/cc 
//      Basic implementation for the CubeSat class
//

#ifndef LORIS_CUBESAT_CUBESAT_H_
#define LORIS_CUBESAT_CUBESAT_H_

#include <vector>

#include "../../terminal/header/Messenger.h"

class CubeSat: public Messenger {
    public:

        CubeSat();

        void SendMessage(std::string message);
        // void AddTaskToQueue(Task task);  // Commented until Task is defined
        // Task CreateTask(std::string task);

    private:
        // std::vector<Task*> tasks_;
};

#endif