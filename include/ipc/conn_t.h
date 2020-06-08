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

typedef struct Conn
{
  int rx; // receiving connection socket
  int tx; // transmitting connection socket
} conn_t;

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CONN_T_H */
