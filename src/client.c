/*
* client.c
*
*   purpose: Uses the client API to connect to the IPC.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

// Standard C Libraries
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
  // Check argc
  if(argc != 2)
  {
    fprintf(stderr, "Invalid number of arguments\n Try: ./client <name>\n");
    return -1;
  }

  // Create placeholder for client name
  char * name = argv[1];

  // Check name length
  if(strlen(name) > 3) // name is too long
  {
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Initialize IPC client interface
  if(ipc_init(name) == -1) // ipc_init()
  {
    fprintf(stderr, "ipc_init() failed\n");
    return -1;
  }

  // done
  return 0;
}
