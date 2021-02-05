/**
 * main.c
 *
 * purpose: daemonize the subsystem dock
 * author:  alex amellal
 *
 */

// Subsystem server container
#include "subsystem_module.h"

// Dock library
#include "dock.h"

// Subsystem modules
#include "template.h"
#include "twin.h"
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
  if (dock_start(modules, modules_len) != 0) {
    fprintf(stderr, "dock_start() failed\n");
    return -1;
  }

  // Wait for interrupt
  fprintf(stdout, "Press [CTRL-C] to stop dock\n");
  signal(SIGINT, isr);

  for (;;) {
    sleep(1);
  }

  // done
  return 0;
}

// Interrupt signal routine handler
void isr(int sig) {
  // Check if modules_ptr null 
  if(modules_ptr == NULL) {
    fprintf(stdout, "\nNothing to do. Exiting!\n");
    exit(0);
  }

  switch (sig) {
    case SIGINT:
      fprintf(stdout, "\nStopping dock daemon...\n");
      dock_stop(modules_ptr, modules_len);
      fprintf(stdout, "Done!\n");
      exit(0);
      break;
  };
}