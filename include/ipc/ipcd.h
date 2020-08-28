/*
* ipcd.h
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_IPC_IPCD_H
#define CUBESAT_CORE_INCLUDE_IPC_IPCD_H

// Project headers 
#include "ipc/settings.h"
#include "util/immut.h"
#include "ipc/client_t.h"

// Standard C libraries 
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

// Private keyword
#include "util/private.h"

// Methods 
int ipcd_init();            // Initialize the IPC daemon
int ipcd_close();           // Shutdown the IPC daemon

// Debug 
int ipcd_print_clients();   // Prints the list of clients

#endif  // end of CUBESAT_CORE_INCLUDE_IPC_IPCD_H header guard.