//
// Created by Spencer Axford on 2019-05-04.
//

#ifndef DALCUBESAT_INTERPROCESSCOMMUNICATIONINTERFACE_H
#define DALCUBESAT_INTERPROCESSCOMMUNICATIONINTERFACE_H

class InterProcessCommunicationInterface {
public:
    virtual int receive(char message[], char source[]) = 0;
    virtual int send(char message[], char destination[]) = 0;
};


#endif //DALCUBESAT_INTERPROCESSCOMMUNICATIONINTERFACE_H
