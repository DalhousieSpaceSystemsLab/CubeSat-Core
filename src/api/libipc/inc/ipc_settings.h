/**
 * @file ipc_settings.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Control default values used in program
 * @version 0.1
 * @date 2021-05-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
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
#define MAX_ARG_LEN 32
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

// Flags
enum {
  IPC_REFRESH_MSG = (1 << 0),
  IPC_REFRESH_RECV = (1 << 1),
  IPC_REFRESH_FLUSH = (1 << 2),
};

enum {
  IPC_READ_DEFAULT = (1 << 0),
  IPC_READ_FNEW = (1 << 1),
};

// JSON
#define JSON_KEY_LEN 32
#define JSON_VAL_LEN 32
#define JSON_TEST_LEN 32
#define JSMN_TOK_OFF 3

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: CUBESAT_CORE_INCLUDE_IPC_SETTINGS_H */
