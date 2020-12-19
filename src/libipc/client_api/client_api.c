/*
* client_api.c
*
*   purpose: provides API for other subsystems to use the IPC system as clients.
*   author: alex amellal
*
*/

// Project headers
#define _POSIX_C_SOURCE 199309L
#include "ipc/client_api.h"

// private variables
static client_t self;  // self-referential placeholder for this client

static char qsend_dest[NAME_LEN];    // send queue destination name
static char qsend_msg[MAX_MSG_LEN];  // send queue message placeholder
static int qsend_msg_len = -1;       // send queue message length

static MsgReqDib dibs[MAX_NUM_DIBS]; // Stores message request dibs 

// Initialize client API and connect to IPC daemon.
int ipc_connect(char name[NAME_LEN]) {
  // Initialize client placeholder for self
  self = client_t_new();

  // Copy name into self
  for (int x = 0; x < NAME_LEN; x++) 
    self.name[x] = name[x];

  // Create placeholder for socket address
  const struct sockaddr_un address = {
      .sun_family = AF_UNIX,
      .sun_path   = "./socket.socket",
  };
  const socklen_t address_len = sizeof(address);

  // Initialize dibs array 
  for(int x = 0; x < MAX_NUM_DIBS; x++) {
    dibs[x] = MsgReqDib_new();
  }

  // Initiate rx socket
  // Nonblocking flag enabled for this socket
  if ((self.conn.rx = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {  // socket() failed
    perror("socket() failed");
    return -1;
  }

  // Initiate tx socket
  if ((self.conn.tx = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {  // socket() failed
    perror("socket() failed");
    return -1;
  }

  // Connect to host (rx first)
  if (connect(self.conn.rx, (struct sockaddr *)&address, address_len) == -1) {  // connect() failed
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if (write(self.conn.rx, name, strlen(name)) < strlen(name)) {  // write() failed
    perror("write() failed");
    return -1;
  }

  // Connect to host (tx next)
  if (connect(self.conn.tx, (struct sockaddr *)&address, address_len) == -1) {  // connect() failed
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if (write(self.conn.tx, name, strlen(name)) < strlen(name)) {  // write() failed
    perror("write() failed");
    return -1;
  }

  // done
  return 0;
}

// Send message to another process
int ipc_send(char dest[NAME_LEN], char *msg, size_t msg_len) {
  // Ensure message is long enough to contain a name
  if (msg_len < 1) {
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
  for (int x = offset; (x - offset) < msg_len; x++) {
    // Copy message character into final message
    msg_final[x] = msg[x - offset];
  }

  // Calculate final message length
  size_t msg_final_len = NAME_LEN + 1 + msg_len;

  // Write message to ipc
  if (write(self.conn.tx, msg_final, msg_final_len) < msg_final_len) {  // write() failed
    perror("write() failed");
    return -1;
  }

  /**
   * If sending receipt confirmation, do not wait for another
   * confirmation.
   * 
   * Otherwise, wait for a confirmation. 
   * If no confirmation is received, consider it a failed transmission.
   */
  if(strcmp(msg, RECV_CONF) == 0) {
    // done 
    return 0;
  }

  // TODO:
  // - Complete receipt confirmation system

  // Listen on IPC for receipt confirmation
  // bool recvd = false;
  // ipc_qrecv(dest, (void (*)(char*,size_t)) {recvd = true});

  // // Wait for receipt confirmation 
  // for(struct timespec t = {.tv_sec=0,.tv_nsec=0}; t.tv_sec < RECV_TIMEOUT.tv_sec && t.tv_nsec < RECV_TIMEOUT.tv_nsec; t.tv_sec += READ_BLOCK_DELAY.tv_sec, t.tv_nsec += READ_BLOCK_DELAY.tv_nsec) {
  //   // Refresh incoming messages from destination
  //   ipc_refresh_src(dest);

  //   // Wait before refreshing again
  //   nanosleep(&READ_BLOCK_DELAY, NULL);
  // }

  // // Remove dib 
  // MsgReqDib_remove(dest, dibs, MAX_NUM_DIBS);

  // // Check if receipt confirmation failed to be received in time
  // if(!recvd) {
  //   fprintf(stderr, "failed to send message, receipt confirmation timed out : ");
  //   return -1;
  // }

  // done
  return 0;
}

// Receive message from another process
// Returns number of bytes of data copied into buffer.
int ipc_recv(char src[NAME_LEN], char *buf, size_t buf_len) {
  // Check if source wildcard 
  bool src_wildcard = strncmp(src, "*", 1) == 0;
  
  // Check if specific source specified 
  if(!src_wildcard) {
    // Check for a preexisting dib on message source
    if(MsgReqDib_exists(src, dibs, MAX_NUM_DIBS)) {
      fprintf(stderr, "unable to claim dibs on message source \"$.*s\" : ", NAME_LEN, src);
      return -1;
    }

    // Since source is free to claim, place a dib on source 
    if(MsgReqDib_add(MsgReqDib_set(src, NULL), dibs, MAX_NUM_DIBS) != 0) {
      fprintf(stderr, "failed to claim dibs on src (%.*s) : ", NAME_LEN, src);
      return -1;
    }
  }

  // Attempt loop
  for(;;) {
    // Create buffer for incoming IPC command 
    char msg[MAX_MSG_LEN];
    memset(msg, 0, MAX_MSG_LEN);

    // Read message from IPC 
    int bytes_read = -1;
    if((bytes_read = read(self.conn.rx, msg, MAX_MSG_LEN)) <= 0) {
      // Check if read should have blocked 
      if(errno == EWOULDBLOCK || errno == EAGAIN) {
        // Wait read delay and try again 
        nanosleep(&READ_BLOCK_DELAY, NULL);
        continue;
      }
      
      // Read failed 
      fprintf(stderr, "read() failed : ");
      return -1;
    }

    // Create placeholders for name 
    char name[NAME_LEN];
    char msg_nameless[MAX_MSG_LEN];
    size_t msg_nameless_len = bytes_read-NAME_LEN-1;

    // Separate name from message 
    strncpy(name, msg, NAME_LEN);
    strncpy(msg_nameless, &msg[NAME_LEN+1], msg_nameless_len);

    //--- Check if message can be claimed ---//

    // Check if source filter is an exact match to incoming message source
    bool exact_match = (strncmp(src, name, NAME_LEN) == 0);

    // Check for no preexisting dibs on incoming message source and wildcard used
    bool preexisting_dibs = MsgReqDib_exists(name, dibs, MAX_NUM_DIBS);

    // Test to see if filter corresponds to message source (exact match or wildcard)
    if(exact_match || (!preexisting_dibs && src_wildcard)) {
      //--- Message CAN be claimed ---//
      
      // Send receipt confirmation 
      if(ipc_send(name, RECV_CONF, strlen(RECV_CONF)) != 0) {
        fprintf(stderr, "ipc_send() failed : ");
        return -1;
      }

      // Remove dibs 
      MsgReqDib_remove(name, dibs, MAX_NUM_DIBS);

      // Copy nameless message into output buffer 
      strncpy(buf, msg_nameless, (buf_len > msg_nameless_len) ? msg_nameless_len : buf_len);

      // Return number of bytes copied 
      return msg_nameless_len;
    
    // claim failed, re-feed into self
    } else {  
      //--- Message CANNOT be claimed ---//

      // Format message to 'self'
      char msg_self[MAX_MSG_LEN];
      strncpy(msg_self, msg, MAX_MSG_LEN);
      memcpy(msg_self, self.name, NAME_LEN);

      // Send message directly to IPC 
      if(write(self.conn.tx, msg_self, bytes_read) < bytes_read) {
        perror("write() failed");
        return -1;
      }

      // Wait read delay and try again 
      nanosleep(&READ_BLOCK_DELAY, NULL);
      continue;
    }
  }

  // done 
  return 0;
}

// Adds outgoing message to send queue
int ipc_qsend(char dest[NAME_LEN], char *msg, size_t msg_len) {
  // Check for null message or 0 length
  if (msg == NULL || msg_len <= 0) {
    // Set qsend message placeholder to 0
    memset(qsend_msg, 0, MAX_MSG_LEN);

    // Set qsend message length to -1
    qsend_msg_len = -1;

    // done
    return 0;
  }

  // Copy destination into queue
  for (int x = 0; x < NAME_LEN; x++) qsend_dest[x] = dest[x];

  // Copy message into queue
  for (int x = 0; x < msg_len && x < MAX_MSG_LEN; x++) qsend_msg[x] = msg[x];

  // Copy message length into queue
  qsend_msg_len = msg_len;

  // done
  return 0;
}

// Adds incoming message request to recv queue
int ipc_qrecv(char src[NAME_LEN], void (*callback)(char*, size_t)) {
  // Check for preexisting dibs on src
  if(MsgReqDib_exists(src, dibs, MAX_NUM_DIBS)) {
    fprintf(stderr, "preexisting dibs on src (%.*s) : ", NAME_LEN, src);
    return -1;
  }

  // Check if callback NULL
  if(callback == NULL) {
    fprintf(stderr, "callback for ipc_qrecv cannot be null : ");
    return -1;
  }

  // Create a new dib for src 
  if(MsgReqDib_add(MsgReqDib_set(src, callback), dibs, MAX_NUM_DIBS) < 0) {
    fprintf(stderr, "MsgReqDib_add() failed : ");
    return -1;
  }

  // done
  return 0;
}

// Simultaneously reads/writes all queued data
int ipc_refresh() {
  return ipc_refresh_src("*");
}

// Simultaneously reads/writes queued data for specific source 
int ipc_refresh_src(char src[NAME_LEN]) {
  //--- QRECV ---//

  // Create placeholder for incoming message from ipc
  char msg[MAX_MSG_LEN];
  int bytes_read = -1;

  // Run single non-blocking read from IPC 
  if((bytes_read = read(self.conn.rx, msg, MAX_MSG_LEN)) < 0) {
    // Make sure read really failed (and didnt try to block)
    if(!(errno == EWOULDBLOCK || errno == EAGAIN)) {
      perror("read() failed");
      return -1;
    }
  }

  // Check if message was received from IPC
  if(bytes_read > NAME_LEN + 2) {
    // Create placeholders for message source name and nameless message 
    char name[NAME_LEN];
    char msg_nameless[MAX_MSG_LEN];
    size_t msg_nameless_len = bytes_read - (NAME_LEN+1);
    
    // Separate source name from message
    strncpy(name, msg, NAME_LEN);
    strncpy(msg_nameless, &msg[NAME_LEN+1], msg_nameless_len);

    // Look for corresponding dibs in dibs array 
    for(int x = 0; x < MAX_NUM_DIBS; x++) {
      
      // Create placeholders for conditions of correspondance
      bool dib_matches_msg_src    = strncmp(name, dibs[x].name, NAME_LEN) == 0;
      bool dib_matches_src_filter = strncmp(src, dibs[x].name, NAME_LEN) == 0;
      bool src_filter_wildcard    = strncmp(src, "*", 1) == 0;
      bool dib_wildcard           = strncmp(dibs[x].name, "*", 1) == 0;
      
      // Check if dib corresponds to dibs rules and src filter
      if((dib_matches_msg_src && dib_matches_src_filter) || (dib_matches_msg_src && src_filter_wildcard) || (dib_wildcard && src_filter_wildcard)) {
        // Check for valid callback 
        if(dibs[x].callback != NULL) {
          // Run callback and pass message as parameter 
          (dibs[x].callback)(msg_nameless, msg_nameless_len);

          // Send receipt confirmation 
          if(ipc_send(dibs[x].name, RECV_CONF, strlen(RECV_CONF)) != 0) {
            fprintf(stderr, "failed to send receipt confirmation : ipc_send() failed : ");
            return -1;
          }

          // done
          break;
        }
      }
    }
  }

  //--- QSEND ---//

  // Check if qsend buffers valid
  if (qsend_msg_len > 0) {  // qsend is good to go
    // Send data
    if (ipc_send(qsend_dest, qsend_msg, qsend_msg_len)) {  // send() failed
      fprintf(stderr, "ipc_send() failed\n");
      return -1;
    }
  }

  // Reset qsend placeholders
  memset(qsend_dest, 0, NAME_LEN);
  memset(qsend_msg, 0, MAX_MSG_LEN);
  qsend_msg_len = -1;

  // done
  return 0;
}

// Disconnect from IPC daemon and close client side interface
int ipc_disconnect() {
  // Send disconnect signal to IPC
  write(self.conn.tx, DISCONNECT_SIG, strlen(DISCONNECT_SIG));

  // Close connection socket to the IPC
  client_t_close(&self);

  // done
  return 0;
}
