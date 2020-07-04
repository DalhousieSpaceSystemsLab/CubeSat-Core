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
static int sock = -1; // connection socket to IPC

// Initialize client API and connect to IPC daemon.
int ipc_connect(char name[3])
{

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

  // Send final message to IPC
  if(write(sock, msg_final, 3 + msg_len) < 3 + msg_len) // write() failed
  {
    fprintf(stderr, "write() failed\n");
    return -1;
  }

  // done
  return 0;
}

// Receive message from another process
int ipc_recv(char src[3], char * buffer, size_t buffer_len)
{
  // Create placeholder for incoming message from IPC
  char msg[MAX_MSG_LEN];

  // Wait for incoming message from the IPC
  int bytes_read = -1;
  if((bytes_read = read(sock, msg, MAX_MSG_LEN)) <= 0) // read() failed or zero length msg
  {
    fprintf(stderr, "Failed to read message from IPC\n");
    return -1;
  }

  // Check if message fits within buffer
  if(buffer_len < bytes_read) // buffer too small
  {
    fprintf(stderr, "Buffer provided to ipc_recv is too small to contain message.\n");
    return -1;
  }

  // Copy message into buffer
  strncpy(buffer, msg, bytes_read);

  // done
  return 0;
}

// Disconnect from IPC daemon and close client side interface
int ipc_disconnect()
{
  // Close connection socket to the IPC
  close(sock);

  // done
  return 0;
}
