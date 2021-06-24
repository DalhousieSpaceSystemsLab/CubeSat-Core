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
#include "subsysmod.h"

// Subsystem modules
#include "command_module.h"
#include "gps_module.h"
#include "mission_module.h"
#include "payload_module.h"
#include "rf_module.h"
#include "test_module.h"

// Standard C libraries
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// Global pointer to server container array
static SubsystemModule* modules_ptr = NULL;
static int modules_len = 0;

int main() {
  // Create list of server containers
  SubsystemModule modules[] = {
      payload, gps, command, mission, rf, test
  };
  modules_len = sizeof(modules) / sizeof(SubsystemModule);

  // Update global pointer to server container array
  modules_ptr = modules;

  // Start dock
  if (dock_start(modules, modules_len) != 0) {
    fprintf(stderr, "dock_start() failed\n");
    return -1;
  }

  // Quit on [ENTER]
  char quitc[3];
  fprintf(stdout, "Press [ENTER] to stop dock\n");
  fgets(quitc, 3, stdin);

  dock_stop(modules, modules_len);

  // done
  return 0;
}