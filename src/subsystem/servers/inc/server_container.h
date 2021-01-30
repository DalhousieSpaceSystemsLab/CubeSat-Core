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
typedef struct server_container {
  void* (*start)();
  void (*stop)();
  pthread_t tid;
  pthread_t mtid;
} ServerContainer;

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H