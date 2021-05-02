/*
 * ipcd.h
 *
 *   purpose: act a middleman between processes to complete communication
 * requests author: alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_IPCD_H
#define CUBESAT_CORE_INCLUDE_IPC_IPCD_H

#ifdef __cplusplus
extern "C" {
#endif

// Project headers
#include "client_t.h"
#include "immut.h"
#include "ipc_packet.h"
#include "ipc_settings.h"

// Standard C libraries
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

// Methods
int ipcd_init();   // Initialize the IPC daemon
int ipcd_close();  // Shutdown the IPC daemon

// Debug
int ipcd_print_clients();  // Prints the list of clients

#ifdef __cplusplus
}
#endif

#endif  // end of CUBESAT_CORE_INCLUDE_IPC_IPCD_H header guard.