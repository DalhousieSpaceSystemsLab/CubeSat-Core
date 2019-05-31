#include "MessageHeader.h"

MessageHeader::MessageHeader(unsigned int recipient, unsigned int sender, long time_created):
        recipient_(recipient), sender_(sender), time_created_(time_created)
{
    // Null
}

unsigned int MessageHeader::GetSender() const 
{
	return this->sender_;
}

unsigned int MessageHeader::GetRecipient() const
{
	return this->recipient_;
}

long MessageHeader::GetTimeCreated() const
{
	return this->time_created_;
}

void MessageHeader::SetSender(unsigned int sender_)
{
	this->sender_ = sender_;
}

void MessageHeader::SetRecipient(unsigned int recipient_)
{
	this->recipient_ = recipient_;
}

void MessageHeader::SetTimeCreated(long time_created_)
{
	this->time_created_ = time_created_;
}
