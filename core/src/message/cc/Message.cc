#include "Message.h"

Message::Message(unsigned int sender, unsigned int recipient, long time, KeyValuePairContainer contents):
        sender_(sender), recipient_(recipient), time_created_(time), contents_(contents)
{
    // Null
}

KeyValuePairContainer Message::GetMessageContents() const
{
	return this->contents_;
}

unsigned int Message::GetSender() const 
{
	return this->sender_;
}

unsigned int Message::GetRecipient() const
{
	return this->recipient_;
}

long Message::GetTimeCreated() const
{
	return this->time_created_;
}

void Message::SetSender(unsigned int sender_)
{
	this->sender_ = sender_;
}

void Message::SetRecipient(unsigned int recipient_)
{
	this->recipient_ = recipient_;
}

void Message::SetTimeCreated(long time_created_)
{
	this->time_created_ = time_created_;
}