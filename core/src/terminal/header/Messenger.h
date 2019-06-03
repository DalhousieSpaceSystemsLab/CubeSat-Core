#ifndef MESSAGENGER_H
#define MESSAGENGER_H

#include <iostream>
#include "Receiver.h"
#include "Transmitter.h"
#include "RFMessage.h"

using namespace std;

/**
 * @brief   : Class for Messages 
 * @params  : message_receiver_    - An instance of a Receiver object
 *          : message_transmitter_ - An instance of a Transmitter object
 */
class Messenger {
    public:
        /**
        * @brief : Null constructor
        */    
        Messenger() {

        }

        /**
         * @brief   : Blank ProcessMessage method
         * @params  : message - An instance of a Message object
         */
        void* ProcessMessage(RFMessage message) {
            
        }
        
        /**
         * @brief   : Blank Transmit method
         * @params  : message - An instance of a Message object
         */
        void Transmit(RFMessage message) {

        }

        /**
         * @brief   : Blank Buildmessage method
         * @params  : message - A string to be constructed into a message
         */
        RFMessage BuildMessage(std::string message) {
            
        }

        Receiver message_receiver_;
        Transmitter message_transmitter_;
};

#endif