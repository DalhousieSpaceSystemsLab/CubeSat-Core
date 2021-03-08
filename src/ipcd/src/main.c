/*
 * main.c
 *
 *   purpose: act a middleman between processes to complete communication
 * requests author: alex amellal
 *
 */

/**
 * @mainpage Welcome to the LORIS core software documentation
 *
 * @section ipc Inter-Process Communication (IPC) System
 * In order to provide a neutral means of communication between all of the
 * different processes and subsystems on the satellite, the IPC system was
 * created.
 *
 * @subsection ipc_client Client API
 * Rather than creating a custom implemention for message passing and handling
 * for every process, a general purpose API for the IPC system was created.
 *
 * The details on how to use the API are found in client_api.h.
 *
 * Examples can be found in <a
 * href="http://cubesat.alexamellal.com/md_src_api_libipc_examples_CLIENT_API_EXAMPLES.html">CLIENT_API_EXAMPLES.md</a>.
 */

// Project Headers
#include "ipcd.h"

// Standard C libraries
#include <stdio.h>

int main() {
  // Initialize IPC daemon
  if (ipcd_init() == -1) {  // ipcd_init() failed
    fprintf(stderr, "ipcd_init() failed\n");
    return -1;
  }

  // Wait for user to press enter
  printf("Press [ENTER] to quit\n");
  getc(stdin);

  // Close the IPC daemon
  if (ipcd_close() == -1) {  // ipcd_close() failed
    fprintf(stderr, "ipcd_close() failed\n");
    return -1;
  }

  // done
  return 0;
}
