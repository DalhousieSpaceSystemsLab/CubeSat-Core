/**
 * @file subsysmod.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief provide a modular placeholder for subsystem modules (container)
 * @version 0.1
 * @date 2021-05-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

// Project headers
#include "client_api.h"
#include "ipc_settings.h"
#include "ipc_std.h"
#include "modutil.h"

// Standard C libraries
#include <pthread.h>
#include <stddef.h>

// Subsystem server container type
typedef struct subsystem_module {
  int (*start)(void*);
  int (*stop)(void*);
  pid_t pid;
  pthread_t mtid;
  char* stack;
  const char* nickname;
} SubsystemModule;

// Module maker macros
#define NEW_DOCK_MODULE(name) extern SubsystemModule name
#define EXPORT_MODULE(name)                             \
  SubsystemModule name = {.start = start_module_##name, \
                          .stop = stop_module_##name,   \
                          .nickname = #name}

#define START_MODULE(name) int start_module_##name(void* data)
#define STOP_MODULE(name) int stop_module_##name(void* data)
#define CALLBACK(name) static int name(char* msg, size_t msg_len, void* data)
#define STOP_CALLBACK return 0

#ifdef __cplusplus
}
#endif

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H