/**
 * subsystem_module.h
 *
 *   purpose: provide a modular placeholder for subsystem modules (container)
 *   author: alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

// Project headers
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
} SubsystemModule;

// Module maker macros
#define NEW_DOCK_MODULE(name) extern SubsystemModule name
#define EXPORT_MODULE(name)                             \
  SubsystemModule name = {.start = start_module_##name, \
                          .stop = stop_module_##name}
#define START_MODULE(name) int start_module_##name(void* data)
#define STOP_MODULE(name) int stop_module_##name(void* data)

#ifdef __cplusplus
}
#endif

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H