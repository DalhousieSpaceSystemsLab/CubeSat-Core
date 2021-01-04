/*
* client_handler.h
*
*   purpose:  handle client connections to the IPC. intended to be used as a thread.
*   author:   alex amellal
*
*/
#ifndef CUBESAT_CORE_INCLUDE_IPC_CLIENT_HANDLER_H
#define CUBESAT_CORE_INCLUDE_IPC_CLIENT_HANDLER_H
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */


// Project Headers
#include "settings.h"
#include "client_t.h"

// Standard C Libraries
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// Set methods
void set_clients(client_t * clients);

// Get methods
int get_free_client();

// Thread methods
void * handle_client_requests(void * client);


#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_HANDLER_H */
