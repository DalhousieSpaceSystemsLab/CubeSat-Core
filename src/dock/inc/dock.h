#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H

#ifdef __cplusplus
extern "C" {
#endif

// Feature test macros
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// Project headers
#include "dock_settings.h"
#include "subsystem_module.h"

// Standard C libraries
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

////////////////////
// Public methods //
////////////////////

// Start running dock and all server instances
int dock_start(SubsystemModule* modules, size_t modules_len);

// Stop dock and all server instances
int dock_stop(SubsystemModule* modules, size_t modules_len);

#ifdef __cplusplus
}
#endif

#endif