/*
 * settings.h
 *
 *   purpose: control default values used in program
 *   author: alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H
#define CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

// IPC
#define MAX_NUM_CLI 16
#define MAX_MSG_LEN 128
#define NAME_LEN 3
#define MAX_ARG_LEN MAX_MSG_LEN - NAME_LEN - 1
#define READ_BLOCK_DELAY \
  (struct timespec) { .tv_sec = 0, .tv_nsec = 100000000 }
#define ROUTER_CHECK_DELAY 1000000  // in nanoseconds
#define DISCONNECT_SIG "DISCONNECT"
#define RECV_CONF "OK"
#define RECV_TIMEOUT 3
#define MAX_NUM_DIBS MAX_NUM_CLI
#define REFEED_DELAY READ_BLOCK_DELAY
#define MAX_NUM_PACKETS 16
#define MAX_PACKET_LEN MAX_MSG_LEN + 5
#define MAX_DIB_STACK 1024

// JSON
#define JSON_KEY_LEN 32
#define JSON_VAL_LEN 32
#define JSON_TEST_LEN 32
#define JSMN_TOK_OFF 3

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H */
