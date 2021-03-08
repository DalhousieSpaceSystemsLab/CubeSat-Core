/**
 * main.c
 *
 * purpose: daemonize the subsystem dock
 * author:  alex amellal
 *
 */

// Subsystem server container
#include "subsystem/subsystem_module.h"

// Dock library
#include "subsystem/dock.h"

// Subsystem servers
#include "subsystem/servers/template_module.h"
#include "subsystem/servers/twin.h"

// Standard C libraries
#include <signal.h>
#include <stdlib.h>

// Interrupt signal routine handler
void isr(int sig);

// Global pointer to server container array
static SubsystemModule* servers_ptr = NULL;
static int servers_len = 0;

int main() {
  // Create list of server containers
  SubsystemModule servers[] = {
      template_server,
      twin_server,
  };
  servers_len = sizeof(servers) / sizeof(SubsystemModule);

  // Update global pointer to server container array
  servers_ptr = servers;

  // Start dock
  if (dock_start(servers, servers_len) != 0) {
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
  // Check if servers_ptr null 
  if(servers_ptr == NULL) {
    fprintf(stdout, "\nNothing to do. Exiting!\n");
    exit(0);
  }

  switch (sig) {
    case SIGINT:
      fprintf(stdout, "\nStopping dock daemon...\n");
      dock_stop(servers_ptr, servers_len);
      fprintf(stdout, "Done!\n");
      exit(0);
      break;
  };
}