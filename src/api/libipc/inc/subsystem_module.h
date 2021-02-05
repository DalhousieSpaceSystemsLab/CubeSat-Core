/**
* subsystem_module.h
*
*   purpose: provide a modular placeholder for subsystem servers (container)
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H

// Project headers
#include "settings.h"

// Standard C libraries
#include <pthread.h>
#include <stddef.h>

// Subsystem server container type
typedef struct subsystem_module {
  int (*start)(void*);
  int (*stop)(void*);
  pid_t pid;
  pthread_t mtid;
  int index;
} SubsystemModule;

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_MODULE_H