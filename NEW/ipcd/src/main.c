/*
* main.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
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
