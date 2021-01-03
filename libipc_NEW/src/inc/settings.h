/*
* settings.h
*
*   purpose: control default values used in program
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H
#define CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H

// IPC
#define MAX_NUM_CLI        5
#define MAX_MSG_LEN        128
#define NAME_LEN           3
#define READ_BLOCK_DELAY   1   // in seconds
#define ROUTER_CHECK_DELAY 1000000 // in nanoseconds
#define DISCONNECT_SIG     "disconnect"

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H */
