/*
* client_api.c
*
*   purpose: provides API for other subsystems to use the IPC system as clients.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

// Private variables
static char name[3];  // client name on IPC
static int sock = -1; // connection socket to IPC

// Initialize client API interface
int ipc_init(char name_[3])
{
  // Set name
  for(int x = 0; x < 3; x++) name[x] = name_[x];

  // Create placeholder for socket address
  const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket",
  };
  socklen_t address_len = sizeof(address);

  // Initiate socket
  if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Connect to host
  if(connect(sock, (struct sockaddr *) &address, address_len) == -1) // connect() failed
  {
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if(write(sock, name, strlen(name)) < strlen(name)) // write() failed
  {
    perror("write() failed");
    return -1;
  }

  // done
  return 0;
}

// Send message to another process
int ipc_send(char dest[3], char * msg, size_t msg_len)
{
  // Create placeholder for message to be sent
  char msg_final[3 + msg_len];

  // Create message with destination client fronted
  sprintf(msg_final, "%.3s %*s\0", dest, msg_len, msg);

  // DEBUG
  printf("Final message is %s\n", msg_final);

  // done
  return 0;
}
