//
// Created by Spencer Axford on 2019-05-16.
//

#include "PowerRepository.h"

PowerRepository::PowerRepository(std::string filePaths)
        : Repository(filePaths) {}

int PowerRepository::ProcessMessage(Message message){
        cout << "Processing Message Object in PowerRepository" << endl;
        KeyValuePairContainer c = message.GetMessageContents();

        cout << "Recipient : Sender : Time Created" << endl;
	cout << message.GetRecipient() << " : " << message.GetSender() << " : " << message.GetTimeCreated() << endl;
        std::vector<int> floatKeys = c.GetFloatKeys();
        std::vector<int> intKeys = c.GetIntKeys();
        cout << "key value pairs:" << endl;
        for(int i = 0; i < intKeys.size(); i++){
                cout << intKeys.at(i) << " : " << c.GetInt(intKeys.at(i)) << endl; 
        }
        for(int i = 0; i < floatKeys.size(); i++){
                cout << floatKeys.at(i) << " : " << c.GetFloat(floatKeys.at(i)) << endl; 
        }
        return 0;
}
