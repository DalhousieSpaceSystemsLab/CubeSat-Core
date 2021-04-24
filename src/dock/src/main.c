/**
 * main.c
 *
 * purpose: daemonize the subsystem dock
 * author:  alex amellal
 *
 */

// Macros
#define _XOPEN_SOURCE

// Project headers
#include "dock.h"
#include "modutil.h"
#include "subsystem_module.h"

// Subsystem modules
#include "mission_module.h"
#include "payload_module.h"

// Standard C libraries
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// Global pointer to server container array
static SubsystemModule* modules_ptr = NULL;
static int modules_len = 0;
static char stacks[MAX_NUM_MODULES][MODULE_STACK_SIZE];

int main() {
  // Create list of server containers
  SubsystemModule modules[] = {
      payload,
      mission,
  };
  modules_len = sizeof(modules) / sizeof(SubsystemModule);

  // Update global pointer to server container array
  modules_ptr = modules;

  // Start dock
  if (dock_start(modules, modules_len, stacks) != 0) {
    fprintf(stderr, "dock_start() failed\n");
    return -1;
  }

  // Quit on [ENTER]
  char quitc[3];
  fprintf(stdout, "Press [ENTER] to stop dock\n");
  fgets(quitc, 3, stdin);

  dock_stop(modules, modules_len, stacks);

  // done
  return 0;
}