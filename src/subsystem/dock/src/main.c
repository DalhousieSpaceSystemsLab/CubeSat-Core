/**
 * main.c
 *
 * purpose: daemonize the subsystem dock
 * author:  alex amellal
 *
 */

// Macros
#define _XOPEN_SOURCE

// Subsystem server container
#include "subsystem_module.h"

// Dock library
#include "dock.h"

// Subsystem modules
// #include "template.h"
// #include "twin.h"
#include "filesystem.h"
#include "payload.h"

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
      // template_server,
      // twin_server,
      filesystem_server,
      payload_server,
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