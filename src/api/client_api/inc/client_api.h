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
#include "ipc_settings.h"
#include "client_t.h"
#include "msg_req_dib.h"
#include "util/immut.h"
#include "json.h"
#include "ipc_packet.h"

// Standard C Libraries
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

enum {
  IPC_QRECV_MSG,
  IPC_QRECV_RECV
};

// Client API
int ipc_connect(const char name[NAME_LEN]);                                                      // Initialize client side IPC interface
int ipc_send(char dest[NAME_LEN], char * msg, size_t msg_len);                                   // Send message to another process
int ipc_send_json(char dest[NAME_LEN], json_t *json, size_t json_len);                           // Sends key-value pair to another process
int ipc_recv(char src[NAME_LEN], char * buffer, size_t buffer_len);                              // Receive message from another process (blocks)
int ipc_qsend(char dest[NAME_LEN], char * msg, size_t msg_len);                                  // Adds outgoing message to send queue
int ipc_qrecv(char src[NAME_LEN], void (*callback)(char*, size_t, void*), void* data, int flags); // Adds incoming message request to recv queue
int ipc_refresh();                                                                        // Simultaneously reads/writes all queued data
int ipc_refresh_src(char src[NAME_LEN]);                                                  // Simultaneously reads/writes queued data for specific source 
int ipc_disconnect();                                                                     // Close client side IPC interface

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H */
