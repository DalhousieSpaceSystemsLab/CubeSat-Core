#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_DOCK_DOCK_H

#ifdef __cplusplus
extern "C" {
#endif

// Feature test macros
#define _GNU_SOURCE

// Project headers
#include "dock_settings.h"
#include "subsystem_module.h"

// Standard C libraries 
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

////////////////////
// Public methods //
////////////////////

// Start running dock and all server instances
int dock_start(SubsystemModule* modules, size_t modules_len, char stacks[MAX_NUM_MODULES][MODULE_STACK_SIZE]);

// Stop dock and all server instances
int dock_stop(SubsystemModule* modules, size_t modules_len, char stacks[MAX_NUM_MODULES][MODULE_STACK_SIZE]);

#ifdef __cplusplus
}
#endif 

#endif