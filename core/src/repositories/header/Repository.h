//
// Created by Spencer Axford on 2019-05-16.
//

#ifndef LORIS_REPOSITORY_H
#define LORIS_REPOSITORY_H

#include "DataMessage.h"
#include "UnixDomainStreamSocketServer.h"
#include <string>

class Repository : public UnixDomainStreamSocketServer {
public:
    Repository(std::string socket_path);
    int HandleMessage(char *buffer);

protected:
    //Keys which indicates what data the repository will contain
    std::vector<Key> watch_list_;

    int AddData(DataMessage message);
private:
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
    virtual int ProcessMessage(DataMessage message) = 0;

    bool WatchListContainsKey(unsigned int key);

};


#endif //LORIS_REPOSITORY_H
