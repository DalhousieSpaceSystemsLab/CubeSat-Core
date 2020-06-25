/*
* client_t.h
*
*   purpose:  defines client connection placeholder structure for IPC.
*   author:   alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_IPC_CLIENT_T_H
#define CUBESAT_CORE_INCLUDE_IPC_CLIENT_T_H

// Connection type
#include "conn_t.h"

// Client type
typedef struct Client
{
  conn_t  conn;     // connection socket placeholders
  char    name[3];  // client name placeholder
} client_t;

// Standardized methods for the client type 
client_t  client_t_new();                     // Returns initialized client_t
int       client_t_close(client_t * client);  // Closes conn member of client_t

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_T_H */
