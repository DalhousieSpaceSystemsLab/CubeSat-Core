/**
 * @file client_api.h
 * @author alex amellal (loris@alexamellal.com)
 * @brief provides API for other subsystems to use the IPC system as clients.
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Dalhousie Space Systems Lab (c) 2021
 * 
 * TODO:
 *  - change ipc_refresh_src dibs mechanism to prioritize dibs with specific sources over wildcard. for
 *    the time being, if a wildcard dib is setup before a specific dib, the wildcard dib will always win.
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

/// QRECV Flags
enum {
  IPC_QRECV_MSG = 0,
  IPC_QRECV_RECV
};

/// IPC Error Values
enum {
  EIPCREAD = -512,
  EIPCPACKET
};

////////////////
// Client API //
////////////////

/**
 * @brief Initialize client side IPC interface. Required to be run for all other IPC commands to work.
 * 
 * @param name 3 letter name you'd like to register as on the IPC network
 * @return 0 = OK, ERROR < 0
 */
int ipc_connect(const char name[NAME_LEN]);                                                      

/**
 * @brief Send message to another process
 * 
 * @param dest Name of destination process
 * @param msg Pointer to character array containing message
 * @param msg_len Length of msg in bytes
 * @return Number of bytes sent. ERROR < 0 
 */
int ipc_send(char dest[NAME_LEN], char * msg, size_t msg_len);

/**
 * @brief Sends key-value pair to another process
 * 
 * @param dest Name of destination process
 * @param json Pointer to JSON key-value pair array
 * @param json_len Number of key-value pairs in json
 * @return Number of bytes sent. ERROR < 0 
 */
int ipc_send_json(char dest[NAME_LEN], json_t *json, size_t json_len);

/**
 * @brief Receive message from another process (blocks)
 * 
 * @param src Message source name filter ("*" = any source)
 * @param buffer Message output buffer (empty character array)
 * @param buffer_len Size of message output buffer in bytes
 * @return number of bytes copied into output buffer 
 */
int ipc_recv(char src[NAME_LEN], char * buffer, size_t buffer_len);

/// Deprecated function
int ipc_qsend(char dest[NAME_LEN], char * msg, size_t msg_len);

/**
 * @brief Creates a background listener for incoming messages from source (with callback function)
 * 
 * @param src Background listener message source filter (from who will the messages be handled by callback?). Use "*"" for any source.
 * @param callback Pointer to callback function (see callback function requirements)
 * @param data Data to be passed to callback function upon execution/triggering (set to NULL if no data needs to be passed)
 * @param flags QRECV flag which determines which dibs array to use (default IPC_QRECV_MSG)
 * @return 0 = OK, ERROR < 0 
 */
int ipc_qrecv(char src[NAME_LEN], void (*callback)(char*, size_t, void*), void* data, int flags); 

/**
 * @brief Refreshes all background message listeners and triggers callback if applicable. See ipc_qrecv(). Equivalent to ipc_refresh_src("*").
 * 
 * @return 0 = OK, ERROR < 0.
 */
int ipc_refresh(); 

/**
 * @brief Refreshes all background message listeners and triggers callback if applicable. See ipc_qrecv().
 * 
 * @param src Specifies which background message listener to refresh. Use "*" for all background listeners.
 * @return 0 = OK, ERROR < 0 
 */
int ipc_refresh_src(char src[NAME_LEN]);

/**
 * @brief Extracts IPC message command and arguments
 * 
 * @param msg Pointer to IPC message to extract.
 * @param msg_len Length of message pointed to by msg in bytes.
 * @param args_out Pointer to array of empty character arrays of at least MAX_ARG_LEN in size each.
 * @param max_args Maximum number of arguments args_out can take.
 * @return Number of args extracted into args_out. ERROR < 0
 */
int ipc_args(char* msg, size_t msg_len, char* args_out[MAX_ARG_LEN], size_t max_args);

/**
 * @brief Close client side IPC interface
 * 
 * @return 0 
 */
int ipc_disconnect();

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H */
