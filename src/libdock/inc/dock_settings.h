/**
 * settings.h
 *
 * purpose: store standardized values for the subsystem module
 * author: alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_DOCK_SETTINGS_H
#define CUBESAT_CORE_INCLUDE_DOCK_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODULE_STACK_SIZE 1024 * 512
#define MONITOR_STACK_SIZE 2 * MODULE_STACK_SIZE
#define MAX_NUM_MODULES 5
#define MODULE_INT_TIMEOUT 3
#define MODULE_STOP_TIMEOUT 1
#define MODULE_REINT_DELAY \
  (struct timespec) { .tv_sec = 0, .tv_nsec = 100000000 }
#define MODULE_START_DELAY \
  (struct timespec) { .tv_sec = 0, .tv_nsec = 100000000 }

#ifdef __cplusplus
}
#endif

#endif  // End of CUBESAT_CORE_INCLUDE_DOCK_SETTINGS_H guard