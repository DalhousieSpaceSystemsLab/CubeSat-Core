#ifndef LORIS_MESSAGE_MESSAGE_H_
#define LORIS_MESSAGE_MESSAGE_H_

#include "KeyValuePairContainer.h"

class Message {
public:
    Message(unsigned int sender, unsigned int recipient);
    Message(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
    Message(char* flat);

    void flatten(char* msg);
    unsigned int GetSender() const;
	unsigned int GetRecipient() const;
	long GetTimeCreated() const;
    KeyValuePairContainer GetMessageContents() const;


	void SetSender(unsigned int sender_);
	void SetRecipient(unsigned int recipient_);
    void SetTimeCreated(long time_created_);

    void poop();

    void Add(unsigned int, int);
    void Add(unsigned int, float);

    void ToString(char* string,int capacity);

private:
    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
    KeyValuePairContainer contents_;

};

#endif
