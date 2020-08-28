/**
* container_t.h
*
*   purpose: provide a modular placeholder for subsystem servers (container)
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H

// Project headers
#include "subsystem/settings.h"

// Standard C libraries
#include <pthread.h>
#include <stddef.h>

// Subsystem server container type
typedef struct SubsystemServerContainer {
  void (*start)();
  void (*stop)();
  pthread_t tid;
} subsystem_server_container_t;

// Marker for end of array of containers (equivalent to EOF for FILE)
#define SUBSYSTEM_SERVER_EOF_ADDR (void*)0x1234
subsystem_server_container_t SUBSYSTEM_SERVER_EOF = {
    .start = SUBSYSTEM_SERVER_EOF_ADDR,
    .stop  = SUBSYSTEM_SERVER_EOF_ADDR,
};

// Standardized methods for subsystem server container type
size_t subsystem_server_container_t_array_len(subsystem_server_container_t* array);

#endif  // END OF CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_H