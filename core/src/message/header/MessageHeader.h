
#ifndef LORIS_MESSAGE_MESSAGEHEADER_H_
#define LORIS_MESSAGE_MESSAGEHEADER_H_

class MessageHeader {
public:
    MessageHeader(unsigned int recipient, unsigned int sender, long time_created);
    
    unsigned int GetSender() const;
	unsigned int GetRecipient() const;
	long GetTimeCreated() const;

	void SetSender(unsigned int sender_);
	void SetRecipient(unsigned int recipient_);
	void SetTimeCreated(long time_created_);
	
private:
    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
};

#endif
