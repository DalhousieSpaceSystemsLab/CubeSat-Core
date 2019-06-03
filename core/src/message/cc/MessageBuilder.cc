#include "MessageBuilder.h"

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

Message MessageBuilder::CompleteMessage()
{
    long current_time = (long)time(NULL);
    return Message(this->current_sender_, this->current_recipient_, current_time, this->message_contents_);
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