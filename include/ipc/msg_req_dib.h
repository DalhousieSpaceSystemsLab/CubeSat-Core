/**
 * msg_req_dib.h 
 * 
 * purpose: defines the placeholder type for message source 'dibs'
 * author:  alex amellal 
 * 
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_MSG_REQ_DIB_H
#define CUBESAT_CORE_INCLUDE_IPC_MSG_REQ_DIB_H

// Project headers 
#include "settings.h"

// Standard C libraries 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Message Request 'Dib' type 
typedef struct msg_req_dib {
  char name[NAME_LEN];
  void (*callback)(char*, size_t);
} MsgReqDib;

// Standardized functions for message request dib 
MsgReqDib MsgReqDib_new();                                                          // Returns blank dib 
MsgReqDib MsgReqDib_set(char name[NAME_LEN], void (*callback)(char*, size_t));      // Returns initialized dib 
int       MsgReqDib_add(MsgReqDib element, MsgReqDib * array, size_t array_len);    // Appends or overwrites dib into dib array
bool      MsgReqDib_exists(char name[NAME_LEN], MsgReqDib *array, size_t array_len); // Checks in array for preexisting dibs
int       MsgReqDib_remove(char name[NAME_LEN], MsgReqDib *array, size_t array_len); // Removes a dib from an array of dibs

#endif 