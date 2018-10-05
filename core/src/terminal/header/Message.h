#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

using namespace std;

/**
 * @brief   : Class for Messages 
 * @params  : contents_ - A string containing the message contents
 */
class Message {
    public:
        /**
        * @brief : Null constructor
        */
        Message();

        /**
         * @brief   : Message constructor
         * @params  : contents - contents to be stored in message
         */
        Message(std::string contents);

        /**
         *  @brief  : Getter for message contents
         *  @return : String containing message contents
         */
        std::string getContents();
        
        std::string contents_;
};

#endif 