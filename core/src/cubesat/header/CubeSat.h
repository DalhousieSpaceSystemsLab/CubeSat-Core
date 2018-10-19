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

        void SendMessageToGroundStation(Message message);
        void ProcessMessage(Message message);

    private:
        
};

#endif