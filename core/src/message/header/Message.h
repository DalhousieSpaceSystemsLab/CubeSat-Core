#ifndef LORIS_MESSAGE_MESSAGE_H_
#define LORIS_MESSAGE_MESSAGE_H_

#include "KeyValuePairContainer.h"

class Message {
public:
    Message(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
    
    KeyValuePairContainer GetMessageContents() const;
    unsigned int GetSender() const;
	unsigned int GetRecipient() const;
	long GetTimeCreated() const;

	void SetSender(unsigned int sender_);
	void SetRecipient(unsigned int recipient_);
    void SetTimeCreated(long time_created_);
private:
    KeyValuePairContainer contents_;
    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
};

#endif
