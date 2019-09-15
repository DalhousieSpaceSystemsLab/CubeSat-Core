//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef LORIS_REPOSITORY_H
#define LORIS_REPOSITORY_H

#include "Message.h"
#include "DataMessage.h"
#include "CommandMessage.h"
#include "MessageReceivingService.h"
#include <string>
#include "Identifiers.h"

class Repository {
public:
    Repository(unsigned int identifier);
    void start();

protected:
    //TODO make watch_list_ a map
    //Keys which indicates what data the repository will contain
    std::vector<Key> watch_list_;

	/**
	 * Replies with return_message to whatever client is currently connected to
	 * the repository
	 * @param return_message The message that will be flattened and sent back
	 * @return
	 */
    int ReplyToConnectedClient(DataMessage& return_message);

    /**
     * Builds a message containing data according to requests
     * @param request_message
     * @param return_message
     * @return
     */
    int BuildReturnDataMessage(DataMessage request_message,
    		DataMessage& return_message);

    /**
     * Offloads data contained in a DataMessage and stores it
     * @param message
     * @return
     */
    int AddData(DataMessage message);

    /**
     *
     * @return Repository Identifier which uniquely identifies the repository
     */
    virtual unsigned int repository_identifier() = 0;

private:
    MessageReceivingService mrs;
    /**
     * Extracts key value pairs which the repository is designated to store
     * @param received_message
     * @return
     */
    int ExtractDataFromReceivedMessage(DataMessage received_message);


    //Keys will be added to the watch_list_ in this function. Must be implemented or else
    //the repository will never accept, store, or return data.
    /*
     * The idea behind a watch list is that the repository will only ever accept incoming data
     * which are listed in the watch list. For example, the Power repository will only
     * accept data which has a key listed in its watch list.
     */
    virtual int AddKeysToWatchList()=0;
    //All data (key value pairs) are stored in here.
    //Only key value pairs with keys in the watch list should be added to it.
    KeyValuePairContainer repository_data_;

    //KeyValuePairContainer repository_data;
    void HandleConnection(int file_descriptor);

    /**
     * Perform additional processing on the message received (optional)
     * @param message
     * @return
     */
    virtual int ProcessMessage(DataMessage message) = 0;
    bool WatchListContainsKey(unsigned int key);
};

#endif //LORIS_REPOSITORY_H
