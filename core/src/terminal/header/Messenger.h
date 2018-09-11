#ifndef MESSAGENGER_H
#define MESSAGENGER_H

#include <iostream>
#include "Receiver.h"
#include "Transmitter.h"
#include "Message.h"

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
        void* ProcessMessage(Message message) {
            
        }
        
        /**
         * @brief   : Blank Transmit method
         * @params  : message - An instance of a Message object
         */
        void Transmit(Message message) {

        }

        /**
         * @brief   : Blank Buildmessage method
         * @params  : message - A string to be constructed into a message
         */
        Message BuildMessage(std::string message) {
            
        }

        Receiver message_receiver_;
        Transmitter message_transmitter_;
};

#endif