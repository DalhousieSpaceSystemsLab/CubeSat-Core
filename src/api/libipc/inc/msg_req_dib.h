/**
 * msg_req_dib.h
 *
 * purpose: defines the placeholder type for message source 'dibs'
 * author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_MSG_REQ_DIB_H
#define CUBESAT_CORE_INCLUDE_IPC_MSG_REQ_DIB_H

#ifdef __cplusplus
extern "C" {
#endif

// Project headers
#include "ipc_settings.h"

// Standard C libraries
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Message Request 'Dib' type
typedef struct msg_req_dib {
  char name[NAME_LEN];
  int (*callback)(char*, size_t, void*);
  void* data;
  pthread_t tid;
} MsgReqDib;

// Packaged callback arguments type
typedef struct dib_callback_args {
  char* msg;
  size_t msg_len;
  void* data;
} DibCallbackArgs;

// Standardized functions for message request dib //

// Returns blank dib
MsgReqDib MsgReqDib_new();

// Returns initialized dib
MsgReqDib MsgReqDib_set(char name[NAME_LEN],
                        int (*callback)(char*, size_t, void*), void* data);

// Appends or overwrites dib into dib array
int MsgReqDib_add(MsgReqDib element, MsgReqDib* array, size_t array_len);

// Checks in array for preexisting dibs
bool MsgReqDib_exists(char name[NAME_LEN], MsgReqDib* array, size_t array_len);

// Checks if callback for dib is running
bool MsgReqDib_is_running(char name[NAME_LEN], MsgReqDib* array,
                          size_t array_len);

// Removes a dib from an array of dibs
int MsgReqDib_remove(char name[NAME_LEN], MsgReqDib* array, size_t array_len);

#ifdef __cplusplus
}
#endif

#endif