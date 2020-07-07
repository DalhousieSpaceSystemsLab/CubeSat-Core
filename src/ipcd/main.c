/*
* main.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*/

// Project Headers
#include "ipc/ipcd.h"

// Standard C libraries
#include <stdio.h>

int main()
{
  // Initialize IPC daemon
  if(ipcd_init() == -1) // ipcd_init() failed
  {
    fprintf(stderr, "ipcd_init() failed\n");
    return -1;
  }

  // Start accepting new clients 
  if(ipcd_start_accepting() == -1) // ipcd_start_accepting() failed 
  {
    fprintf(stderr, "ipcd_start_accepting() failed\n");
    return -1;
  }

  // Start routing messages between clients 
  if(ipcd_start_routing() == -1) // ipcd_start_routing() failed 
  {
    fprintf(stderr, "ipcd_start_routing() failed");
    return -1;
  }

  for(;;)
  {
    ipcd_print_clients();

    printf("Press [ENTER] to print again or 'q' to stop printing.\n");
    char input = getc(stdin);
    if(input == 'q') break;  
  }

  // Wait for user to press enter 
  printf("Press [ENTER] to quit\n");
  getc(stdin);

  // Close the IPC daemon
  if(ipcd_close() == -1) // ipcd_close() failed
  {
    fprintf(stderr, "ipcd_close() failed\n");
    return -1;
  }

  // done
  return 0;
}
