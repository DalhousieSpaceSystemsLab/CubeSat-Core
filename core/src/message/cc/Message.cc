#include "Message.h"

Message::Message(MessageHeader header, KeyValuePairContainer contents):
        header_(header), contents_(contents)
{
    // Null
}