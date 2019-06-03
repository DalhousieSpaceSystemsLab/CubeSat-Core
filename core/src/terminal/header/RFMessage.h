#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

using namespace std;

/**
 * @brief   : Class for Messages 
 * @params  : contents_ - A string containing the RFMessage contents
 */
class RFMessage {
    public:
        /**
        * @brief : Null constructor
        */
        RFMessage();

        /**
         * @brief   : RFMessage constructor
         * @params  : contents - contents to be stored in RFMessage
         */
        RFMessage(std::string contents);

        /**
         *  @brief  : Getter for RFMessage contents
         *  @return : String containing RFMessage contents
         */
        std::string getContents() const;
        
        std::string contents_;
};

#endif 