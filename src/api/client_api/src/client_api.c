/*
 * client_api.c
 *
 *   purpose: provides API for other subsystems to use the IPC system as
 * clients. author: alex amellal
 *
 *   TODO:
 */

// Project headers
#define _POSIX_C_SOURCE 199309L
#include "client_api.h"

// Private constants
enum { IPC_WRITE_NORM, IPC_WRITE_REFEED };

// Private variables
static client_t self;  // self-referential placeholder for this client
static char qsend_dest[NAME_LEN];              // send queue destination name
static char qsend_msg[MAX_MSG_LEN];            // send queue message placeholder
static int qsend_msg_len = -1;                 // send queue message length
static MsgReqDib dibs[MAX_NUM_DIBS];           // Stores message request dibs
static MsgReqDib recv_dibs[MAX_NUM_DIBS];      // Stores read-receipt dibs
static ipc_packet_t packets[MAX_NUM_PACKETS];  // Incoming packet queue

//////////////////////
// Callback methods //
//////////////////////

// Callback which is used by ipc_recv
static int cb_recv(char *, size_t, void *);

// Thread wrapper for callback methods
static int cb_thread(void *data);

/////////////////////
// Private methods //
/////////////////////

// wraps write() function with custom packetizing
static int ipc_write(char dest[NAME_LEN], char *msg, size_t msg_len, int flags);

// wraps read() function with custom packetizing
static int ipc_read(char src_out[NAME_LEN], char *buffer, size_t buffer_len);

// Initialize client API and connect to IPC daemon.
int ipc_connect(const char name[NAME_LEN]) {
  // Initialize client placeholder for self
  self = client_t_new();

  // Copy name into self
  for (int x = 0; x < NAME_LEN; x++) self.name[x] = name[x];

  // Create placeholder for socket address
  const struct sockaddr_un address = {
      .sun_family = AF_UNIX,
      .sun_path = "./socket.socket",
  };
  const socklen_t address_len = sizeof(address);

  // Initialize dibs array
  for (int x = 0; x < MAX_NUM_DIBS; x++) {
    dibs[x] = MsgReqDib_new();
  }

  // Initiate rx socket
  // Nonblocking flag enabled for this socket
  if ((self.conn.rx = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
    // socket() failed
    perror("socket() failed");
    return -1;
  }

  // Initiate tx socket
  if ((self.conn.tx = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    // socket() failed
    perror("socket() failed");
    return -1;
  }

  // Connect to host (rx first)
  if (connect(self.conn.rx, (struct sockaddr *)&address, address_len) == -1) {
    // connect() failed
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if (write(self.conn.rx, name, strlen(name)) < strlen(name)) {
    // write() failed
    perror("write() failed");
    return -1;
  }

  // Connect to host (tx next)
  if (connect(self.conn.tx, (struct sockaddr *)&address, address_len) == -1) {
    // connect() failed
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if (write(self.conn.tx, name, strlen(name)) < strlen(name)) {
    // write() failed
    perror("write() failed");
    return -1;
  }

  // done
  return 0;
}

// Wraps write() function with custom packetizing
// RETURN number of bytes sent to IPC.
static int ipc_write(char dest[NAME_LEN], char *msg, size_t msg_len,
                     int flags) {
  // Create placeholder for outgoing data
  char data_out[MAX_PACKET_LEN];

  // Used whenever write() cannot write the entire packet in one piece
  int cursor = 0;
  int total_bytes_written = 0;

  // Format outgoing data
  int data_out_len =
      sprintf(data_out, "%.*s <%.*s>", NAME_LEN, dest, msg_len, msg);

  // Write data to the IPC
  int bytes_written = 0;
  while ((bytes_written = write(self.conn.tx, &data_out[cursor],
                                data_out_len)) < data_out_len) {
    if (bytes_written < 0) {
      perror("write() failed");
      return -1;
    } else {
      cursor = bytes_written;
      data_out_len -= cursor;
      total_bytes_written += bytes_written;
      continue;
    }
  }

  // Update total bytes written
  total_bytes_written += bytes_written;

  // done
  return total_bytes_written;
}

// Send message to another process
int ipc_send(char dest[NAME_LEN], char *msg, size_t msg_len) {
  // Ensure message is long enough to contain a name
  if (msg_len < 1) {
    fprintf(stderr,
            "ignoring ipc_send request for message that is too short\n");
    return -1;
  }

  // Write message to ipc
  // if (write(self.conn.tx, msg_final, msg_final_len) < msg_final_len) {  //
  // write() failed
  if (ipc_write(dest, msg, msg_len, IPC_WRITE_NORM) <
      msg_len) {  // write() failed
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
  if (strcmp(msg, RECV_CONF) == 0) {
    // done
    return 0;
  }

  // Create placeholder for receipt confirmation
  char recv_conf[MAX_MSG_LEN];
  bool recvd = false;

  // Listen for incoming receipt confirmation
  if (ipc_qrecv(dest, cb_recv, recv_conf, IPC_QRECV_RECV) != 0) {
    fprintf(stderr, "ipc_qrecv() failed : ");
    return -1;
  }

  // Initialize time placeholders for timeout
  time_t start, current, time_elapsed = 0;
  time(&start);

  // Refresh message queue until timeout exceeded
  for (int x = 0; time_elapsed < RECV_TIMEOUT; x++) {
    // Refresh message queue
    if (ipc_refresh_src(dest) != 0) {
      fprintf(stderr, "ipc_refresh_src() failed : ");
      return -1;
    }

    // Check receipt confirmation placeholder
    if (strncmp(recv_conf, RECV_CONF, strlen(RECV_CONF)) == 0) {
      recvd = true;
      break;
    }

    // Update time elapsed
    time(&current);
    time_elapsed = current - start;
  }

  // Remove dib
  MsgReqDib_remove(dest, recv_dibs, MAX_NUM_DIBS);

  // Check if receipt confirmation arrived before timeout
  if (!recvd) {
    fprintf(stderr, "failed to get receipt confirmation before timeout : ");
    return -1;
  }

  // done
  return 0;
}

// Send command to destination
int ipc_send_cmd(const char *dest, const char *cmd) {
  return ipc_send((char *)dest, (char *)cmd, strlen(cmd));
}

// Sends key-value pair to another process
int ipc_send_json(char dest[NAME_LEN], json_t *json, size_t json_len) {
  // Convert JSON struct into string
  char json_str[MAX_MSG_LEN];
  size_t json_str_len = 0;
  if ((json_str_len = json_stringify(json, json_len, json_str, MAX_MSG_LEN)) <
      0) {
    fprintf(stderr, "json_stringify() failed : ");
    return -1;
  }

  // Send JSON string over IPC
  return ipc_send(dest, json_str, json_str_len);
}

// Wraps read() function with custom packetizing
// RETURN number of bytes read from IPC
static int ipc_read(char src_out[NAME_LEN], char *buffer, size_t buffer_len) {
  // Check if any packets waiting in queue
  if (ipc_packet_waiting(packets, MAX_NUM_PACKETS)) {
    // Pop packet from queue
    ipc_packet_t packet = ipc_packet_pop(packets, MAX_NUM_PACKETS);

    // Check if packet blank
    if (!ipc_packet_blank(packet)) {
      // Export packet to string buffers
      int msg_len = ipc_packet_export(packet, src_out, buffer, buffer_len);

      // done
      return msg_len;
    }
  }

  // Read data from IPC
  char incoming_data[MAX_PACKET_LEN];
  int incoming_data_len = 0;
  if ((incoming_data_len = read(self.conn.rx, incoming_data, MAX_PACKET_LEN)) <
      0) {
    // Make sure read really failed (and didnt try to block)
    if (!(errno == EWOULDBLOCK || errno == EAGAIN)) {
      perror("read() failed");
      return EIPCREAD;
    } else {
      return EIPCREAD;
    }
  }

  // Parse data into packets
  ipc_packet_t next_packet;
  char overflow[MAX_PACKET_LEN];
  int overflow_len = -1;
  for (int x = 0; x < MAX_NUM_PACKETS; x++) {
    // Attempt to parse data
    overflow_len = ipc_packet_parse(incoming_data, incoming_data_len,
                                    &next_packet, overflow);

    // Catch parsing errors
    if (overflow_len < 0) {
      fprintf(stderr, "ipc_packet_parse() failed : ");
      return EIPCPACKET;
    }

    // Add parsed packet to queue
    if (ipc_packet_add(packets, MAX_NUM_PACKETS, next_packet) < 0) {
      fprintf(stderr, "ipc_packet_add() failed : ");
      return EIPCPACKET;
    }

    // Check for overflow
    if (overflow_len) {
      // Replace original data with overflow data
      strncpy(incoming_data, overflow, overflow_len);
      incoming_data_len = overflow_len;
      continue;

    } else
      break;
  }

  // Pop packet from queue and export to buffers
  ipc_packet_t packet = ipc_packet_pop(packets, MAX_NUM_PACKETS);
  int msg_len = ipc_packet_export(packet, src_out, buffer, buffer_len);

  // done
  return msg_len;
}

// Receive message from another process
// Returns number of bytes of data copied into buffer.
// int ipc_recv(char src[NAME_LEN], char *buf, size_t buf_len) {
//   // Check if source wildcard
//   bool src_wildcard = strncmp(src, "*", 1) == 0;

//   // Check if specific source specified
//   if (!src_wildcard) {
//     // Check for a preexisting dib on message source
//     if (MsgReqDib_exists(src, dibs, MAX_NUM_DIBS)) {
//       fprintf(stderr,
//               "unable to claim dibs on message source \"$.*s\" : ", NAME_LEN,
//               src);
//       return -1;
//     }

//     // Since source is free to claim, place a non-callback dib on source
//     if (MsgReqDib_add(MsgReqDib_set(src, NULL, NULL), dibs, MAX_NUM_DIBS) !=
//         0) {
//       fprintf(stderr, "failed to claim dibs on src (%.*s) : ", NAME_LEN,
//       src); return -1;
//     }
//   }

//   // Attempt loop
//   for (;;) {
//     // Read message from IPC
//     char name[NAME_LEN];
//     char msg[MAX_MSG_LEN];
//     int msg_len = 0;
//     if ((msg_len = ipc_read(name, msg, MAX_MSG_LEN)) <= 0) {
//       // Check if read should have blocked
//       if (errno == EWOULDBLOCK || errno == EAGAIN) {
//         // Wait read delay and try again
//         nanosleep(&READ_BLOCK_DELAY, NULL);
//         continue;
//       }

//       // Read failed
//       fprintf(stderr, "ipc_read() failed : ");
//       return -1;
//     }

//     //--- Check if message can be claimed ---//

//     // Check if source filter is an exact match to incoming message source
//     bool exact_match = (strncmp(src, name, NAME_LEN) == 0);

//     // Check for no preexisting dibs on incoming message source and wildcard
//     // used
//     bool preexisting_dibs = MsgReqDib_exists(name, dibs, MAX_NUM_DIBS);

//     // Test to see if filter corresponds to message source (exact match or
//     // wildcard)
//     if (exact_match || (!preexisting_dibs && src_wildcard)) {
//       //--- Message CAN be claimed ---//

//       // Ensure incoming message is NOT receipt confirmation
//       if (!(strncmp(msg, RECV_CONF, strlen(RECV_CONF)) == 0)) {
//         // Send receipt confirmation
//         if (ipc_send(name, RECV_CONF, strlen(RECV_CONF)) != 0) {
//           fprintf(stderr, "ipc_send() failed : ");
//           return -1;
//         }
//       }

//       // Remove dibs
//       MsgReqDib_remove(name, dibs, MAX_NUM_DIBS);

//       // Copy nameless message into output buffer
//       strncpy(buf, msg, (msg_len < buf_len) ? msg_len : buf_len);

//       // Return number of bytes copied
//       return msg_len;

//       // claim failed, re-feed into self
//     } else {
//       //--- Message CANNOT be claimed ---//

//       // Refeed message into self
//       if (ipc_write(name, msg, msg_len, IPC_WRITE_REFEED) < msg_len) {
//         perror("ipc_write() failed");
//         return -1;
//       }

//       // Wait read delay and try again
//       nanosleep(&READ_BLOCK_DELAY, NULL);
//       continue;
//     }
//   }

//   // done
//   return 0;
// }
int ipc_recv(char src[NAME_LEN], char *buf, size_t buf_len) {
  // Create dib for source
  char msg[MAX_MSG_LEN] = {'\0'};
  if (ipc_create_listener(src, cb_recv, (void *)msg) < 0) {
    fprintf(stderr, "failed to create listener for ipc_recv : ");
    return -1;
  }

  // Refresh source until message is received
  for (;;) {
    if (ipc_refresh() < 0) {
      fprintf(stderr, "failed to refresh incoming messages : ");
      ipc_remove_listener(src);
      return -1;
    }

    if (strlen(msg) > 0) {
      strncpy(buf, msg, buf_len < strlen(msg) ? buf_len : strlen(msg));
      break;
    }
  }

  // Remove listener
  if (ipc_remove_listener(src) < 0) {
    fprintf(stderr, "failed to remove listener for ipc_recv : ");
    return -1;
  }

  // done
  return strlen(msg);
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
int ipc_qrecv(char src[NAME_LEN], int (*callback)(char *, size_t, void *),
              void *data, int flags) {
  // Set which dibs array to refer to
  MsgReqDib *dibs_array;
  if (flags == IPC_QRECV_MSG)
    dibs_array = dibs;
  else
    dibs_array = recv_dibs;

  // Check for preexisting dibs on src
  if (MsgReqDib_exists(src, dibs_array, MAX_NUM_DIBS)) {
    fprintf(stderr, "preexisting dibs on src (%.*s) : ", NAME_LEN, src);
    return -1;
  }

  // Check if callback NULL
  if (callback == NULL) {
    fprintf(stderr, "callback for ipc_qrecv cannot be null : ");
    return -1;
  }

  // Create a new dib for src
  if (MsgReqDib_add(MsgReqDib_set(src, callback, data), dibs_array,
                    MAX_NUM_DIBS) < 0) {
    fprintf(stderr, "MsgReqDib_add() failed : ");
    return -1;
  }

  // done
  return 0;
}

// Creates background listener for incoming messages
int ipc_create_listener(char src[NAME_LEN],
                        int (*callback)(char *, size_t, void *), void *data) {
  return ipc_qrecv(src, callback, data, IPC_QRECV_MSG);
}

// Removes background listener(s) with matching source name.
int ipc_remove_listener(char src[NAME_LEN]) {
  // Remove dibs
  return MsgReqDib_remove(src, dibs, MAX_NUM_DIBS);
}

// Simultaneously reads/writes all queued data
int ipc_refresh() { return ipc_refresh_src("*"); }

// Simultaneously reads/writes queued data for specific source
int ipc_refresh_src(char src[NAME_LEN]) {
  //--- QRECV ---//

  // Create placeholder for incoming message from ipc
  char name[NAME_LEN];
  char msg_raw[MAX_MSG_LEN];
  char msg[MAX_MSG_LEN];
  int msg_len = -1;

  // Run single non-blocking read from IPC
  // if((bytes_read = read(self.conn.rx, msg_raw, MAX_MSG_LEN)) < 0) {
  if ((msg_len = ipc_read(name, msg, MAX_MSG_LEN)) < 0) {
    if (msg_len == EIPCPACKET) {
      fprintf(stderr, "ipc_read() failed, packet error : ");
      return -1;
    } else if (!(errno == EWOULDBLOCK || errno == EAGAIN)) {
      perror("read() failed");
      return -1;
    }
  }

  // Check if message was received from IPC
  if (msg_len > 0) {
    // Check if incoming message is a receipt conf
    MsgReqDib *dibs_array;
    if (strncmp(msg, RECV_CONF, strlen(RECV_CONF)) == 0) {
      dibs_array = recv_dibs;
    } else {
      dibs_array = dibs;
    }

    // Look for corresponding dibs in dibs array
    for (int x = 0; x < MAX_NUM_DIBS; x++) {
      // Create placeholders for conditions of correspondance
      bool dib_matches_msg_src =
          strncmp(name, dibs_array[x].name, NAME_LEN) == 0;
      bool dib_matches_src_filter =
          strncmp(src, dibs_array[x].name, NAME_LEN) == 0;
      bool src_filter_wildcard = strncmp(src, "*", 1) == 0;
      bool dib_wildcard = strncmp(dibs_array[x].name, "*", 1) == 0;

      // Check if dib corresponds to dibs rules and src filter
      if ((dib_matches_msg_src && dib_matches_src_filter) ||
          (dib_matches_msg_src && src_filter_wildcard) ||
          (dib_wildcard && src_filter_wildcard)) {
        // Check for valid callback
        if (dibs_array[x].callback != NULL) {
          // Check if dib callback currently running
          if (MsgReqDib_is_running(dibs_array[x])) {
            // Wait for the dib to stop
            int status;
            waitpid(dibs_array[x].pid, &status, 0);

            // Check if callback failed to terminate
            if (!WIFEXITED(status) && !WIFSIGNALED(status)) {
              // Go to the next dib
              continue;
            }

            // Reset dib callback
            MsgReqDib_stop_callback(&dibs_array[x]);
          }

          // Run callback on new thread
          DibCallbackArgs args = {.callback = dibs_array[x].callback,
                                  .msg = msg,
                                  .msg_len = msg_len,
                                  .data = dibs_array[x].data};
          dibs_array[x].pid =
              clone(cb_thread, dibs_array[x].stack, SIGCHLD, (void *)&args);

          // Make sure message is NOT a receipt confirmation
          if (strncmp(msg, RECV_CONF, msg_len) != 0) {
            // Send receipt confirmation
            if (ipc_send(name, RECV_CONF, strlen(RECV_CONF)) != 0) {
              fprintf(
                  stderr,
                  "failed to send receipt confirmation : ipc_send() failed : ");
              return -1;
            }
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

  // Add delay if continuously refreshing
  nanosleep(&READ_BLOCK_DELAY, NULL);

  // done
  return 0;
}

// Extracts IPC message command and arguments
int ipc_args(char *msg, size_t msg_len, char args_out[][MAX_ARG_LEN],
             size_t max_args) {
  // Create placeholders for parsing
  int argc = 0;
  int argx = 0;

  // Check if message contains JSON
  if (json_test(msg, msg_len)) {
    // Treat entire message as one argument
    strncpy(args_out[argc], msg, msg_len < MAX_ARG_LEN ? msg_len : MAX_ARG_LEN);
    argc = 1;
  } else {
    for (int msgx = 0; msgx < msg_len && argc < max_args; msgx++) {
      // Move to next argument if whitespace encountered
      if (msg[msgx] == ' ') {
        // Add null termination character
        args_out[argc][argx] = '\0';

        // Move counters to new argument
        argc++;
        argx = 0;
        continue;
      } else {
        // Otherwise add sequential characters to current argument
        args_out[argc][argx] = msg[msgx];
        argx++;

        // If argument full of characters, force new argument trigger on next
        // iteration
        if (argx >= MAX_ARG_LEN) {
          msg[msgx + 1] = ' ';
          continue;
        }
      }
    }
  }

  // done
  // return argc+1 unless max_args exceeded (meaning last arg was not added)
  return argc <= max_args ? argc + 1 : max_args;
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

// Callback for ipc_recv
static int cb_recv(char *msg, size_t msg_len, void *data) {
  // Copy incoming message into data
  strncpy((char *)data, msg, msg_len);
}

// Thread wrapper for callback methods
static int cb_thread(void *data) {
  // Extract args from void pointer
  DibCallbackArgs args = *((DibCallbackArgs *)data);

  // Run callback
  args.callback(args.msg, args.msg_len, args.data);

  return 0;
}