// /*
// * client_api.h
// *
// *   purpose: provides API for other subsystems to use the IPC system as clients.
// *   author: alex amellal
// *
// */

// #ifndef CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H
// #define CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H

// // Project headers
// #include "ipc/settings.h"
// #include "ipc/client_t.h"
// #include "ipc/msg_req_dib.h"
// #include "util/immut.h"

// // Standard C Libraries
// #include <stdio.h>
// #include <sys/socket.h>
// #include <sys/un.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <errno.h>
// #include <time.h>
// #include <stdbool.h>

// // Client API
// int ipc_connect(char name[3]);                                                            // Initialize client side IPC interface
// int ipc_send(char dest[3], char * msg, size_t msg_len);                                   // Send message to another process
// int ipc_recv(char src[3], char * buffer, size_t buffer_len);                              // Receive message from another process (blocks)
// int ipc_qsend(char dest[3], char * msg, size_t msg_len);                                  // Adds outgoing message to send queue
// int ipc_qrecv(char src[3], void (*callback)(char*, size_t, void*), void* data);                              // Adds incoming message request to recv queue
// int ipc_refresh();                                                                        // Simultaneously reads/writes all queued data
// int ipc_refresh_src(char src[NAME_LEN]);                                                  // Simultaneously reads/writes queued data for specific source 
// int ipc_disconnect();                                                                     // Close client side IPC interface

// #endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_CLIENT_API_H */