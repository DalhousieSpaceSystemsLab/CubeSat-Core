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
static client_t self;               // self-referential placeholder for this client

static char qsend_dest[NAME_LEN];   // send queue destination name 
static char qsend_msg[NAME_LEN];    // send queue message placeholder

static char   qrecv_src[NAME_LEN];  // receive queue source name filter 
static char * qrecv_msg = NULL;     // receive queue message placeholder 
static size_t qrecv_msg_len = -1;   // receive queue message 

// Initialize client API and connect to IPC daemon.
int ipc_connect(char name[NAME_LEN])
{
  // Initialize client placeholder for self 
  self = client_t_new();

  // Copy name into self 
  for(int x = 0; x < NAME_LEN; x++) self.name[x] = name[x];
  
  // Create placeholder for socket address
  const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket",
  };
  const socklen_t address_len = sizeof(address);

  // Initiate rx/tx sockets
  if((self.conn.rx = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  if((self.conn.tx = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Connect to host (rx first)
  if(connect(self.conn.rx, (struct sockaddr *) &address, address_len) == -1) // connect() failed
  {
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if(write(self.conn.rx, name, strlen(name)) < strlen(name)) // write() failed
  {
    perror("write() failed");
    return -1;
  }

  // Connect to host (tx next)
  if(connect(self.conn.tx, (struct sockaddr *) &address, address_len) == -1) // connect() failed
  {
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if(write(self.conn.tx, name, strlen(name)) < strlen(name)) // write() failed
  {
    perror("write() failed");
    return -1;
  }

  // done
  return 0;
}

// Send message to another process
int ipc_send(char dest[NAME_LEN], char * msg, size_t msg_len)
{
  // Create placeholder for message to be sent
  char msg_final[3 + msg_len + 1];

  // Create message with destination client fronted
  sprintf(msg_final, "%.3s %*s\0", dest, msg_len, msg);

  // Send final message to IPC
  if(write(self.conn.tx, msg_final, 3 + msg_len) < 3 + msg_len) // write() failed
  {
    fprintf(stderr, "write() failed : ");
    return -1;
  }

  // done
  return 0;
}

// Receive message from another process
int ipc_recv(char src[NAME_LEN], char * buffer, size_t buffer_len)
{
  // Create placeholder for incoming message from IPC
  char msg[MAX_MSG_LEN];

  // Wait for incoming message from the IPC
  int bytes_read = -1;
  if((bytes_read = read(self.conn.rx, msg, MAX_MSG_LEN)) <= 0) // read() failed or zero length msg
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

// Adds outgoing message to send queue
int ipc_qsend(char dest[NAME_LEN], char * msg, size_t msg_len)
{
  // Copy destination into queue
  for(int x = 0; x < NAME_LEN; x++) qsend_dest[x] = dest[x];

  // Copy message into queue
  for(int x = 0; x < msg_len && x < MAX_MSG_LEN; x++) qsend_msg[x] = msg[x];
  
  // Add null termination character to message if message is shorter than max
  if(msg_len < MAX_MSG_LEN) qsend_msg[msg_len] = '\0';

  // done
  return 0;
}

// Adds incoming message request to recv queue
int ipc_qrecv(char src[NAME_LEN], char * buffer, size_t buffer_len)
{
  // Copy src filter into queue 
  for(int x = 0; x < NAME_LEN; x++) qrecv_src[x] = src[x];

  // Copy buffer data into queue 
  qrecv_msg = buffer;
  qrecv_msg_len = buffer_len;

  // done
  return 0;
} 

// Disconnect from IPC daemon and close client side interface
int ipc_disconnect()
{
  // Close connection socket to the IPC
  client_t_close(&self);

  // done
  return 0;
}
