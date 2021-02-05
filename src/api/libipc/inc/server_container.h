/**
* server_container.h
*
*   purpose: provide a modular placeholder for subsystem servers (container)
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H

// Project headers
#include "settings.h"

// Standard C libraries
#include <pthread.h>
#include <stddef.h>

// Subsystem server container type
typedef struct subsystem_module {
  int (*start)(void*);
  int (*stop)(void*);
  int pid;
  pthread_t mtid;
} SubsystemModule;

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H