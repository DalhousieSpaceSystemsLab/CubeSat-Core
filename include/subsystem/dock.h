#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H

// Project headers
#include "subsystem/server_container.h"
#include "subsystem/servers/template.h"
#include "subsystem/servers/twin.h"

// Standard C libraries 
#include <pthread.h>
#include <errno.h>

// Public methods
int dock_start(ServerContainer* servers, size_t servers_len);  // Start running dock and all server instances
int dock_stop(ServerContainer * servers, size_t servers_len);  // Stop dock and all server instances

#endif