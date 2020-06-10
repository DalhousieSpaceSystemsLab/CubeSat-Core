/*
* client_api.h
*
*   purpose: provides API for other subsystems to use the IPC system as clients.
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H
#define CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H

// Project headers
#include "settings.h"

// Standard C Libraries
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

////////////////
//  C Version //
////////////////

#ifdef __cplusplus
extern "C" {
#endif

// Client API
int ipc_init(char name[3]);                                 // Initialize client side IPC interface
int ipc_send(char dest[3], char * msg, size_t msg_len);     // Send message to another process
int ipc_recv(char src[3], char * buffer, size_t buffer_len);  // Receive message from another process
int ipc_close();                                            // Close client side IPC interface

#ifdef __cplusplus
}
#endif

//////////////////
//  C++ Version //
//////////////////

#ifdef __cplusplus

// Standard C++ Libraries
#include <map>
#include <iterator>
#include <string>
#include <exception>

// Standard C++ classes
using std::map;
using std::pair;
using std::string;
using std::exception;

// Client API (C++)
namespace dss
{
  namespace ipc
  {
    // Methods
    void    init(string name);
    void    send(string dest, string msg);
    string  recv(string src);
    void    ipc_send_map(char dest[3], map<string, string> * map);
    void    ipc_send_map(char dest[3], map<string, int> * map);
    void    ipc_send_map(char dest[3], map<int, int> * map);
    void    close();
  };
};

#endif

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H */
