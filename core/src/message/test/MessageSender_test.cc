//
// Created by Spencer Axford on 2019-05-16.
//
#include <iostream>
//#include "MessageSerializer.h"
#include "Message.h"
#include "MessageBuilder.h"
#include "MessageSenderInterface.h"
#include "MessageSerializer.h"
#include "KeyValuePairContainer.h"

int main(int argc, char *argv[])
{
    cout << "Starting program" << endl;

    MessageBuilder messageBuilder;
    messageBuilder.StartMessage();

    KeyValuePairContainer container;

    unsigned int floatKey = 1;
    unsigned int intKey = 2;
    float floatValue = 300.508;
    int intValue = 500;

    container.AddKeyValuePair(floatKey, floatValue);
    container.AddKeyValuePair(intKey, intValue);

    float containerFloatValue = container.GetFloat(0);
    int containerIntValue = container.GetInt(0);

    cout << "floatValue: " << floatValue << endl << "containerFloatValue: " << containerFloatValue << endl;
    if (containerFloatValue == floatValue) {
        cout << "Container float value successfully added and retrieved" << endl;
    } else {
        cout << "ERROR: Container float value NOT successfully added and retrieved" << endl;
    }

    cout << "intValue: " << intValue << endl << "containerIntValue: " << containerIntValue << endl;
    if (containerIntValue == intValue) {
        cout << "Container int value successfully added and retrieved" << endl;
    } else {
        cout << "ERROR: Container int value NOT successfully added and retrieved" << endl;
    }

    messageBuilder.SetMessageContents(container);
    messageBuilder.SetRecipient(1);
    messageBuilder.SetSender(2);

    Message message = messageBuilder.CompleteMessage();
    
    char data[10000];
    SerializeMessage(&message, data);
    MessageSenderInterface ms(message.GetHeader().GetRecipient());
    ms.SendMessage(data);
    return 0;
}
