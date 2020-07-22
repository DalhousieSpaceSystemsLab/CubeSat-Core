/*
* client.c
*
*   purpose: Uses the client API to connect to the IPC.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

int main(int argc, char * argv[])
{
  // Check argc
  // if(argc != 2)
  if(argc != 3)
  {
    fprintf(stderr, "Invalid number of arguments\n Try: ./client <name> <read/write>\n");
    return -1;
  }

  // Create placeholder for client name
  char * name = argv[1];
  char * rdwr = argv[2];

  // Check name length
  if(strlen(name) != 3) // name is not correct length
  {
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Connect to IPC system
  if(ipc_connect(name) == -1) // ipc_connect() failed
  {
    fprintf(stderr, "Failed to connect to the ipc\n");
    return -1;  
  }

  // Check if client reading or writing 
  if(strcmp(rdwr, "read") == 0) // reading
  {
    // Create placeholder for incoming message 
    char msg[MAX_MSG_LEN];

    // Read data 
    int bytes_read = -1;
    if((bytes_read = ipc_recv("*", msg, MAX_MSG_LEN)) == -1) // ipc_recv() failed 
    {
      fprintf(stderr, "ipc_recv() failed\n");
      return -1;
    }

    // Print data 
    printf("message received: %s\n", msg);
  } 
  
  else if(strcmp(rdwr, "write") == 0) // writing 
  {
    // Create placeholder for message to send 
    char msg[MAX_MSG_LEN];

    // Create message to send 
    sprintf(msg, "how's it going?");

    // Send message to other client 
    if(ipc_send("rrr", msg, strlen(msg)) == -1) // ipc_send() failed 
    {
      fprintf(stderr, "ipc_send() failed\n");
      return -1;
    }
  }

  else // bad keyword 
  {
    fprintf(stderr, "invalid setting. try ./client <name> <read/write>\n");
    return -1;
  }

  // Close IPC 
  ipc_disconnect();

  // done
  return 0;
}