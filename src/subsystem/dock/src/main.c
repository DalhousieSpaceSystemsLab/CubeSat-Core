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

// Interrupt signal routine handler
void isr(int sig);

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

  // Listen for interrupt
  struct sigaction sa = {
    .sa_handler = isr
  };
  sigaction(SIGINT, &sa, NULL);

  // Quit on [ENTER]
  char quitc[3];
  fprintf(stdout, "Press [ENTER] to stop dock\n");
  fgets(quitc, 3, stdin);

  dock_stop(modules, modules_len, stacks);

  // done
  return 0;
}

// Interrupt signal routine handler
void isr(int sig) {
  // Check if modules_ptr null 
  if(modules_ptr == NULL || stacks == NULL) {
    fprintf(stdout, "\nNothing to do. Exiting!\n");
    exit(0);
  }

  switch (sig) {
    case SIGINT:
      fprintf(stdout, "\nStopping dock daemon...\n");
      dock_stop(modules_ptr, modules_len, stacks);
      fprintf(stdout, "Done!\n");
      exit(0);
      break;
  };
}