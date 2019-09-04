//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_PHONEBOOK_H
#define LORIS_PHONEBOOK_H

#include "Identifiers.h"
#include "ProcessFilePaths.h"
#include "DataMessage.h"
#include <string>

// Converts identifier code to socket file path (if it exists)
class PhoneBook {
private: 
    //Constructor
    PhoneBook();
private:

public:
    //Converts Identifier (as int) to the filepath of the Unix Domain Socket
    static std::string IdentifierToProcessFilePath(unsigned int identifier);
};


#endif //LORIS_PHONEBOOK_H
