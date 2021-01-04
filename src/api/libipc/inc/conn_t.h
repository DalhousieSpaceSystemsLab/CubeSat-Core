/*
* conn_t.h
*
*   purpose:  defines placeholder for 2 connection sockets
*             to enable asynchronous two-way communication.
*   author:   alex amellal
*
*/
#ifndef CUBESAT_CORE_INCLUDE_IPC_CONN_T_H
#define CUBESAT_CORE_INCLUDE_IPC_CONN_T_H
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */


// Standard C libraries 
#include <unistd.h>

// Connection type
typedef struct Conn
{
  int rx; // receiving connection socket
  int tx; // transmitting connection socket
} conn_t;

// Standardized methods for the connection type 
conn_t  conn_t_new();                 // Returns initialized client_t
int     conn_t_stat();                // Returns connection status (0 = connected, -1 = closed).
int     conn_t_close(conn_t * conn);  // Closes connection


#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CONN_T_H */
