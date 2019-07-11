#include <iostream>
#include "Message.h"
#include "MessageBuilder.h"
#include "MessageSenderInterface.h"

#include "LorisMessenger.h"
#include "PowerKeys.h"
#include "Identifiers.h"
using namespace std;

int main() {
	//Initialize the messenger
	LorisMessenger tester;

	//Add your data (arguments are: identifier for data, data value)
	PowerKeys power_keys;//Identifier for data generated from EPS

	tester.Add(power_keys.current_sensor1,0.2f);//reading from a current sensor
	tester.Add(power_keys.battery_level,1);//battery level


	//Look up the identifier for the recipient (in this case, its a process known as the "power repository")
	Identifiers recipients;

	//The second argument here is the ID of whoever initiated sending the message.
	tester.Send(recipients.power_repository,0);


}


int old_test(){
    cout << "Starting program" << endl;

    MessageBuilder messageBuilder;
    messageBuilder.StartMessage();

    KeyValuePairContainer container;

    unsigned int floatKey = 1;
    unsigned int floatKey2 = 23;
    unsigned int intKey = 2;
    float floatValue = -300.508;
    float floatValue2 = 4063.3534;
    int intValue = 500;

    container.AddKeyValuePair(floatKey, floatValue);
    container.AddKeyValuePair(floatKey2, floatValue2);
    container.AddKeyValuePair(intKey, intValue);

    float containerFloatValue = container.GetFloat(0);
    float containerFloatValue2 = container.GetFloat(1);
    int containerIntValue = container.GetInt(0);

    cout << "floatValue: " << floatValue << endl << "containerFloatValue: " << containerFloatValue << endl;
    if (containerFloatValue == floatValue) {
        cout << "Container float value successfully added and retrieved" << endl;
    } else {
        cout << "ERROR: Container float value NOT successfully added and retrieved" << endl;
    }

    cout << "floatValue2: " << floatValue2 << endl << "containerFloatValue2: " << containerFloatValue2 << endl;
    if (containerFloatValue == floatValue) {
        cout << "Second container float value successfully added and retrieved" << endl;
    } else {
        cout << "ERROR: Second container float value NOT successfully added and retrieved" << endl;
    }

    cout << "intValue: " << intValue << endl << "containerIntValue: " << containerIntValue << endl;
    if (containerIntValue == intValue) {
        cout << "Container int value successfully added and retrieved" << endl;
    } else {
        cout << "ERROR: Container int value NOT successfully added and retrieved" << endl;
    }

    messageBuilder.SetMessageContents(container);
    messageBuilder.SetRecipient(3025893);
    messageBuilder.SetSender(4589013);

    Message message = messageBuilder.CompleteMessage();

    // SerializeMessage(&message, msg);
    MessageSenderInterface ms(message.GetRecipient());
    ms.SendMessage(message);

    char msg[255] = "";
    message.flatten(msg);
    Message De_message = Message(msg);

    std::vector<int> keys = message.GetMessageContents().GetKeys();

    KeyValuePairContainer c = De_message.GetMessageContents();
	cout << De_message.GetRecipient() << " : " << De_message.GetSender() << " : " << De_message.GetTimeCreated() << endl;
    cout << keys[0] << "-" << c.GetFloat(0) << " : " << keys[1] << "-" << c.GetFloat(1) << " : " << keys[2] << "-" << c.GetInt(0) << endl; 

    return 0;
}
