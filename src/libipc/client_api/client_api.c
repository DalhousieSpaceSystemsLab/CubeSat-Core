/*
* client_api.c
*
*   purpose: provides API for other subsystems to use the IPC system as clients.
*   author: alex amellal
*
*   TODO:
*   - Rewrite ipc_recv function. real buggy
*/

// Project headers
#define _POSIX_C_SOURCE 199309L
#include "ipc/client_api.h"

// private variables
static client_t self;  // self-referential placeholder for this client

static char qsend_dest[NAME_LEN];    // send queue destination name
static char qsend_msg[MAX_MSG_LEN];  // send queue message placeholder
static int qsend_msg_len = -1;       // send queue message length

static char qrecv_src[NAME_LEN];  // receive queue source name filter
static char *qrecv_buf   = NULL;  // receive queue message placeholder
static int qrecv_buf_len = -1;    // receive queue placeholder length

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

  // Create placeholders for timeout counters
  // char recv_conf_msg[MAX_MSG_LEN];
  // long int time_el = 0;
  // long int time_el_max = RECV_TIMEOUT.tv_sec*1000000000L + RECV_TIMEOUT.tv_nsec;
  // long int time_inc = READ_BLOCK_DELAY.tv_sec*1000000000L + READ_BLOCK_DELAY.tv_nsec;
  
  // // Wait for receipt confirmation 
  // for(time_el = 0; time_el < time_el_max; time_el += time_inc) {
  //   // Check if read fails 
  //   if(read(self.conn.rx, recv_conf_msg, MAX_MSG_LEN) <= 0) {
  //     // Check if read should have blocked 
  //     if(errno == EWOULDBLOCK || errno == EAGAIN) {
  //       // Wait & try again
  //       nanosleep(&READ_BLOCK_DELAY, NULL);
  //       continue;
  //     } else {
  //       // Something actually went wrong 
  //       perror("read() failed");
  //       return -1;
  //     }
  //   } else {
  //     // read succeeded, get out of loop 
  //     break;
  //   }
  // }

  // // Check if receipt confirmation timeout exceeded 
  // if(time_el >= RECV_TIMEOUT.tv_sec*1000000000L + RECV_TIMEOUT.tv_nsec) {
  //   fprintf(stderr, "ipc_send receipt confirmation timed out : ");
  //   return -1;
  // }

  // done
  return 0;
}

// // Receive message from another process
// // Returns number of bytes of data copied into buffer.
// int ipc_recv(char src[NAME_LEN], char *buffer, size_t buffer_len) {
//   // Create placeholder for dib 
//   MsgReqDib dib = MsgReqDib_new();

//   // Check if source name specified 
//   if(strncmp(src, "*", 1) != 0) {
//     // Check if dib already exists for source 
//     if(MsgReqDib_exists(src, dibs, MAX_NUM_DIBS)) {
//       // Dont bother doing anything, source is already claimed 
//       fprintf(stderr, "dibs on IPC source \"%.3s\" already exists : ");
//       return -1;
//     }

//     // Create dib 
//     dib = MsgReqDib_set(src, NULL);

//     // Add to dibs 
//     if(MsgReqDib_add(dib, dibs, MAX_NUM_DIBS) != 0) {
//       fprintf(stderr, "MsgReqDib_add() failed : ");
//       return -1;
//     }
//   }
  
//   // Create placeholder for incoming message from IPC
//   char msg[MAX_MSG_LEN];

//   // Create placeholder for source name and message
//   char name[NAME_LEN];
//   char msg_final[MAX_MSG_LEN];
//   size_t msg_final_len = 0;
  
//   // Block until message is received WITHOUT EXISTING DIBS
//   bool msg_has_dibs = false;
//   do {
//     // Block until message is received from the IPC
//     int bytes_read = -1;
//     while((bytes_read = read(self.conn.rx, msg, MAX_MSG_LEN)) <= 0) {
//       // Check if read should have blocked 
//       if(errno == EWOULDBLOCK || errno == EAGAIN) {
//         // Delay next read() attempt
//         nanosleep(&READ_BLOCK_DELAY, NULL);

//         // Try again
//         continue;
//       }

//       // Otherwise, read really failed 
//       perror("read() failed");
//       return -1;
//     }

//     // Separate message from source name 
//     strncpy(name, msg, NAME_LEN);
//     strncpy(msg_final, &msg[NAME_LEN + 1], bytes_read - (NAME_LEN + 1));

//     // Set final message length 
//     msg_final_len = bytes_read;

//     // Message processing for catch-all filter 
//     if(strncmp(src, "*", 1) == 0) {
//       // Search for existing dibs 
//       msg_has_dibs = MsgReqDib_exists(name, dibs, MAX_NUM_DIBS);
      
//       // Check if dibs found
//       if(msg_has_dibs) {
//         // Format 'send to myself' IPC message 
//         char msg_refeed[MAX_MSG_LEN];
//         strncpy(msg_refeed, self.name, NAME_LEN);
//         strncpy(&msg_refeed[NAME_LEN + 1], msg, bytes_read - (NAME_LEN + 1));

//         // Refeed into IPC 
//         if(write(self.conn.tx, msg_refeed, bytes_read) < bytes_read) {
//           perror("write() failed");
//           return -1;
//         }

//         // Allow time for message to processed by another thread 
//         nanosleep(&REFEED_DELAY, NULL);
//       }

//     // Message filter is NOT catch-all
//     } else { 
//       // Check if source filter corresponds 
//       if(strncmp(src, name, NAME_LEN) == 0) {
//         // stop checking for dibs
//         break; 
      
//       // Source filter does NOT correspond
//       } else {
//         // Set message has dibs flag 
//         // NOTE: this is only in the potential sense. 
//         //       the existence of dibs for the real 
//         //       source is not guaranteed.
//         msg_has_dibs = true;

//         // Format 'send to myself' IPC message 
//         char msg_refeed[MAX_MSG_LEN];
//         strncpy(msg_refeed, self.name, NAME_LEN);
//         strncpy(&msg_refeed[NAME_LEN + 1], msg, bytes_read - (NAME_LEN + 1));

//         // Refeed into IPC 
//         if(write(self.conn.tx, msg_refeed, bytes_read) < bytes_read) {
//           perror("write() failed");
//           return -1;
//         }

//         // Allow time for message to processed by another thread 
//         nanosleep(&REFEED_DELAY, NULL);
//       }
//     }

//   } while(msg_has_dibs);

//   // Create placeholder for bytes copied into buffer
//   int bytes_copied = 0;

//   // Copy message into buffer
//   for (int x = 0; x < buffer_len && x < msg_final_len; x++) {
//     // Copy character into buffer
//     buffer[x] = msg_final[x];

//     // Update bytes copied
//     bytes_copied++;
//   }

//   // Send receipt confirmation
//   if(ipc_send(name, RECV_CONF, strlen(RECV_CONF)) != 0) {
//     fprintf(stderr, "ipc_send() failed : ");
//     return -1;
//   }

//   // Remove dibs 
//   if(MsgReqDib_remove(src, dibs, MAX_NUM_DIBS) != 0) {
//     fprintf(stderr, "failed to remove dib from dibs array : ");
//     return -1;
//   }

//   // done
//   return bytes_copied;
// }

// Receive message from another process
// Returns number of bytes of data copied into buffer.
int ipc_recv(char src[NAME_LEN], char *buf, size_t buf_len) {
  // Check if specific source specified 
  if(strncmp(src, "*", 1) != 0) {
    // Check if is a preexisting dib on message source
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

    // Create placeholders for message source filter conditions //
    // Check if source filter is an exact match to incoming message source
    bool exact_match = (strncmp(src, name, NAME_LEN) == 0);
    // Check for no preexisting dibs on incoming message source and wildcard used
    bool wildcard_allowed = !MsgReqDib_exists(name, dibs, MAX_NUM_DIBS) && (strncmp(src, "*", 1) == 0);

    // Test to see if filter corresponds to message source (exact match or wildcard)
    if(exact_match || wildcard_allowed) {
      // Send receipt confirmation 
      if(ipc_send(name, RECV_CONF, strlen(RECV_CONF)) != 0) {
        fprintf(stderr, "ipc_send() failed : ");
        return -1;
      }

      // Copy nameless message into output buffer 
      strncpy(buf, msg_nameless, (buf_len > msg_nameless_len) ? msg_nameless_len : buf_len);
      
      // Remove dibs 
      MsgReqDib_remove(name, dibs, MAX_NUM_DIBS);

      // Return number of bytes copied 
      return msg_nameless_len;
    
    // claim failed, re-feed into self
    } else {  
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
int ipc_qrecv(char src[NAME_LEN], char *buf, size_t buf_len, void (*callback)(char*, size_t)) {
  // Copy src filter into queue
  for (int x = 0; x < NAME_LEN; x++) 
    qrecv_src[x] = src[x];

  // Set receive queue buffer pointer
  qrecv_buf = buf;

  // Set receive queue buffer length
  qrecv_buf_len = buf_len;

  // Check if source name specified 
  if(src[0] != '*') { 
    // Create dib for source name 
    MsgReqDib dib = MsgReqDib_set(src, callback);

    // Add dib to array 
    if(MsgReqDib_add(dib, dibs, MAX_NUM_DIBS) == -1) {
      fprintf(stderr, "MsgReqDib_add() failed : ");
      return -1;
    }
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
  // Check if read queued
  if (qrecv_buf != NULL) {
    // Read data
    int bytes_read = -1;
    if ((bytes_read = read(self.conn.rx, qrecv_buf, (qrecv_buf_len > MAX_MSG_LEN ? MAX_MSG_LEN : qrecv_buf_len))) <= 0) {  // read() failed
      // Check if read() should have blocked
      if (errno == EWOULDBLOCK || errno == EAGAIN) {  // read() should have blocked
        // no issue, just continue

        // Set buffer value to 0
        memset(qrecv_buf, 0, qrecv_buf_len);

      } else {  // read() really failed
        perror("read() failed");
        return -1;
      }
    } else {
      // Create placeholders for name and separated message 
      char name[NAME_LEN];
      char msg_final[MAX_MSG_LEN];

      // Separate name from message 
      strncpy(name, qrecv_buf, NAME_LEN);
      for (int x = NAME_LEN + 1; x < bytes_read; x++) 
        msg_final[x - (NAME_LEN + 1)] = qrecv_buf[x];

      // Check if refresh source filter wildcard or matches message source name 
      if(src[0] == '*' || strncmp(src, name, NAME_LEN) == 0) {
        // Check if dibs claimed on source 
        for(int x = 0; x < MAX_NUM_DIBS; x++) {
          // Check if name matches 
          if(strncmp(dibs[x].name, name, NAME_LEN) == 0) {
            // Check if callback present 
            if(dibs[x].callback != NULL) {
              // Run callback 
              dibs[x].callback(qrecv_buf, bytes_read);

              // Set qrecv buffer value to 0
              // NOTE: This is because the message was 'consumed' by the callback
              memset(qrecv_buf, 0, qrecv_buf_len);
            }

            // Clear dib 
            dibs[x] = MsgReqDib_new();

            // Stop checking for name matches 
            break;
          }
        }

      // Check if specific source specified but doesnt match message source 
      } else if(src[0] != '*' && strncmp(src, name, NAME_LEN) != 0) {
        // Message belongs to another function call.
        // Refeed into IPC 
        if(ipc_send(self.name, qrecv_buf, bytes_read) != 0) {
          fprintf(stderr, "ipc_send() failed : ");
          return -1;
        }

        // Clear qrecv buffer 
        // NOTE: This is because the message was 'consumed' by the refeed
        memset(qrecv_buf, 0, qrecv_buf_len);

        // Wait some time to allow refeed to reach another function 
        nanosleep(&REFEED_DELAY, NULL);
      }
    }
  }

  // Reset read queue placeholders
  memset(qrecv_src, 0, NAME_LEN);
  qrecv_buf     = NULL;
  qrecv_buf_len = -1;

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
