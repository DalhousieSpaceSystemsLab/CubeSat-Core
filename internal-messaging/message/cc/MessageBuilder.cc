#include "MessageBuilder.h"
#include "Message.h"
#include "DataMessage.h"
#include "CommandMessage.h"

MessageBuilder::MessageBuilder()
{
    // Null
}

void MessageBuilder::StartMessage()
{
    this->current_recipient_ = 0;
    this->current_sender_ = 0;
    this->message_contents_.~KeyValuePairContainer();
}

DataMessage MessageBuilder::CompleteMessage()
{
    long current_time = (long)time(NULL);
    return DataMessage(this->current_sender_, this->current_recipient_, current_time, this->message_contents_);
}

int MessageBuilder::BuildMessageFromFlattened(Message *&message, string flattened_message, unsigned int max_capacity){
    //count through section delimiters to find flag
    int section_delimiters = 0;
    string flag_string = "";
    for(char& c : flattened_message){
        if(section_delimiters == 3 && c != *section_break){ // Flag char is found
            flag_string += c;
        }
        else if(c == *section_break){
            section_delimiters++;
        }
        if(section_delimiters >= 4){
            break;
        }
    }
    int flag = std::stoi(flag_string, 0, 16); // Convert flag_string to base 10 flag
    if(flag == 100){ //Data Message
        DataMessage *dm = new DataMessage(flattened_message, max_capacity);
        message = static_cast<Message*>(dm);
        return 1;
    }
    else if(flag == 200){ //Command Message
        CommandMessage cm(flattened_message, max_capacity);
        message = static_cast<Message*>(&cm);
        return 1;
    }
    else{ // Unknown Message Type
        return 0;
    }
}

void MessageBuilder::SetRecipient(unsigned int recipient)
{
    this->current_recipient_ = recipient;
}

void MessageBuilder::SetSender(unsigned int sender)
{
    this->current_sender_ = sender;
}

void MessageBuilder::SetMessageContents(KeyValuePairContainer container)
{
    this->message_contents_ = container;
}