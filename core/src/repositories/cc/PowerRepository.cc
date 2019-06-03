//
// Created by Spencer Axford on 2019-05-16.
//

#include "PowerRepository.h"
#include "Repository.cc"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {}

int PowerRepository::ProcessMessage(Message message){
        cout << "Processing Message Object in PowerRepository" << endl;
        KeyValuePairContainer c = message.GetMessageContents();

	cout << message.GetRecipient() << " : " << message.GetSender() << " : " << message.GetTimeCreated() << endl;
        cout << c.GetFloat(0) << " : " << c.GetInt(0) << endl; 
        return 0;
}
