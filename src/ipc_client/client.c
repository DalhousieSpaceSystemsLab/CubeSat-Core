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
  if(argc < 2)
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

  // Connect to IPC daemon 
  if(ipc_connect(name) == -1) // ipc_connect() failed 
  {
    fprintf(stderr, "ipc_connect() failed\n");
    return -1;
  }

  // Check if read argument provided 
  if(argc == 3 && strcmp(argv[2], "1") == 0)
  {
    // Try a simple read/write from IPC 
    char msg[MAX_MSG_LEN];
    if(ipc_recv("*", msg, MAX_MSG_LEN) == -1) // ipc_recv() failed 
    {
      fprintf(stderr, "ipc_recv() failed\n");
      return -1;
    }
    printf("Message received from IPC is %.*s\n", MAX_MSG_LEN, msg);
  }

  char dest[NAME_LEN + 2]; // additional 2 bytes needed for appended "\n\0"
  char msg_out[MAX_MSG_LEN];
  printf("Enter the message destination: ");
  fgets(dest, NAME_LEN + 2, stdin);
  printf("Enter a message to send now: ");
  fgets(msg_out, MAX_MSG_LEN, stdin);

  if(ipc_send(dest, msg_out, MAX_MSG_LEN) == -1) // ipc_send() failed 
  {
    fprintf(stderr, "ipc_send() failed\n");
    return -1;
  }

  // Disconnect from IPC daemon 
  if(ipc_disconnect() == -1) // ipc_disconnect() failed 
  {
    fprintf(stderr, "ipc_disconnect() failed\n");
    return -1;
  }

  // done
  return 0;
}