#include "MessageHeader.h"

MessageHeader::MessageHeader(unsigned int recipient, unsigned int sender, long time_created):
        recipient_(recipient), sender_(sender), time_created_(time_created)
{
    // Null
}