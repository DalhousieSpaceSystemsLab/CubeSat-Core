#include "Message.h"

Message::Message(MessageHeader header, KeyValuePairContainer contents):
        header_(header), contents_(contents)
{
    // Null
}

MessageHeader Message::GetHeader() const
{
	return this->header_;
}

KeyValuePairContainer Message::GetMessageContents() const
{
	return this->contents_;
}
