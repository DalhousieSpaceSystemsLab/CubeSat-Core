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
static char qsend_msg[MAX_MSG_LEN]; // send queue message placeholder
static size_t qsend_msg_len = -1;   // send queue message length

static char   qrecv_src[NAME_LEN];  // receive queue source name filter 
static char * qrecv_buf = NULL;     // receive queue message placeholder 
static size_t qrecv_buf_len = -1;   // receive queue message placeholder length

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

  // Initiate rx socket
  // Nonblocking flag enabled for this socket
  if((self.conn.rx = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Initiate tx socket
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
  // Ensure message is long enough to contain a name 
  if(msg_len < NAME_LEN)
  {
    fprintf(stderr, "ignoring ipc_send request for message that is too short\n");
    return -1;
  }

  // Create placeholder for message to send 
  char msg_final[MAX_MSG_LEN];

  // Copy destination name into final message 
  strncpy(msg_final, dest, NAME_LEN);

  // Add space between destination name and message 
  msg_final[NAME_LEN] = ' ';

  // Create placeholder for msg offset 
  int offset = NAME_LEN + 1;

  // Copy message into final message 
  for(int x = offset; (x - offset) < msg_len; x++)
  {
    // Copy message character into final message 
    msg_final[x] = msg[x - offset];
  }

  // Calculate final message length 
  size_t msg_final_len = NAME_LEN + msg_len;

  // Write message to ipc 
  if(write(self.conn.tx, msg_final, msg_final_len) < msg_final_len) // write() failed 
  {
    perror("write() failed");
    return -1;
  }

  // done
  return 0;
}

// Receive message from another process
// Returns number of bytes of data copied into buffer.
int ipc_recv(char src[NAME_LEN], char * buffer, size_t buffer_len)
{
  // Create placeholder for incoming message from IPC
  char msg[MAX_MSG_LEN];

  // Wait for incoming message from the IPC
  int bytes_read = -1;
  while((bytes_read = read(self.conn.rx, msg, MAX_MSG_LEN)) <= 0) // read() failed 
  {
    // Check if read() should have blocked 
    if(errno == EWOULDBLOCK || errno == EAGAIN) // read() should have blocked
    {
      // Delay next read() attempt  
      sleep(READ_BLOCK_DELAY); 

      // Try again
      continue;
    } 

    else // read() really failed
    {  
      perror("read() failed");
      return -1;
    }
  }

  // Create placeholder for bytes copied into buffer 
  int bytes_copied = 0;

  // Copy message into buffer
  for(int x = 0; x < buffer_len && x < bytes_read; x++) 
  {
    // Copy character into buffer 
    buffer[x] = msg[x]; 

    // Update bytes copied
    bytes_copied++;
  }

  // done
  return bytes_copied;
}

// Adds outgoing message to send queue
int ipc_qsend(char dest[NAME_LEN], char * msg, size_t msg_len)
{
  // Check for null message or 0 length 
  if(msg == NULL || msg_len <= 0) 
  {
    // Set qsend message placeholder to 0
    memset(qsend_msg, 0, MAX_MSG_LEN);

    // Set qsend message length to -1 
    qsend_msg_len = -1;
    
    // done
    return 0;
  }

  // Copy destination into queue
  for(int x = 0; x < NAME_LEN; x++) qsend_dest[x] = dest[x];

  // Copy message into queue
  for(int x = 0; x < msg_len && x < MAX_MSG_LEN; x++) qsend_msg[x] = msg[x];

  // Copy message length into queue 
  qsend_msg_len = msg_len;

  // done
  return 0;
}

// Adds incoming message request to recv queue
int ipc_qrecv(char src[NAME_LEN], char * buffer, size_t buffer_len)
{
  // Copy src filter into queue 
  for(int x = 0; x < NAME_LEN; x++) qrecv_src[x] = src[x];

  // Copy buffer data into queue 
  qrecv_buf = buffer;
  qrecv_buf_len = buffer_len;

  // done
  return 0;
} 

// Simultaneously reads/writes queued data
int ipc_refresh()
{
  // Check if qrecv buffer valid 
  if(qrecv_buf != NULL && qrecv_buf_len > -1) // qrecv is good to go
  {
    // Read data
    if(read(self.conn.rx, qrecv_buf, qrecv_buf_len) <= 0) // read() failed 
    {
      // Check if read() should have blocked 
      if(errno == EWOULDBLOCK || errno == EAGAIN) // read() should have blocked
      {
        // no issue, just continue 
      }

      else // read() really failed  
      {
        perror("read() failed");
        return -1;
      }
    }
  }

  // Check if qsend buffers valid
  if(qsend_msg_len > 0) // qsend is good to go
  {
    // Send data 
    if(write(self.conn.tx, qsend_msg, MAX_MSG_LEN) < MAX_MSG_LEN) // send() failed 
    {
      perror("send() failed");
      return -1;
    }
  }

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
