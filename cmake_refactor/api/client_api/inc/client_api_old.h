/*
 * client_api.h
 *
 *   purpose: provides API for other subsystems to use the IPC system as
 * clients. author: alex amellal
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

// Initialize client side IPC interface
int ipc_init(const char *name);

// Send message to another process
int ipc_send(const char *dest, char *msg, size_t msg_len);

// Receive message from another process
int ipc_recv(const char *src, char *buffer, size_t buffer_len);

// Close client side IPC interface
int ipc_close();

#ifdef __cplusplus
}
#endif

//////////////////
//  C++ Version //
//////////////////

#ifdef __cplusplus

#include "client_api.hpp"

#endif

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H */
