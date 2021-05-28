/**
* ipcd.c
*
*   purpose: act a middleman between processes to complete communication
requests
*   author: alex amellal
*
*   TODO:
    - C++ wrappers -- may be worth prioritizing other things
    - Queue multiple messages
    - Setup unit testing with new functions

    BUGS:
    - Re-feeding messages into IPC eventually erases message. Why?
*
*/

// Project headers
#include "ipcd.h"
#include <stdarg.h>  // stdarg has to be included here for some reason

#define LOG_TRAFFIC 1

///////////////////////
//  Local Variables  //
///////////////////////

// Placeholder for connected clients
static client_t clients[MAX_NUM_CLI];

// Placeholder for incoming connection socket
static int sock_ = -1;
static immut(int) sock = &sock_;

// Placeholder for incoming packets
static ipc_packet_t packets[MAX_NUM_PACKETS];

//////////////////////
//  Static Methods //
//////////////////////

static void *start_accepting(void *debug);
static void *start_routing_client(void *params);
static void disconnect_client(client_t *client);
static void log_traffic(char src[NAME_LEN], char dest[NAME_LEN],
                        char msg[MAX_PACKET_LEN], size_t msg_len);
static void ipcd_printf(const char *msg, ...);

// Thread which processes incoming client connections
static void *start_accepting(void *debug) {
  for (;;) {
    // Create placeholder for client connection
    int conn = -1;

    // Accept new client
    if ((conn = accept(val(sock), NULL, NULL)) == -1) {  // accept() failed
      perror("start_accepting() : accept() failed");
      pthread_exit(NULL);
    }

    // Get client name
    char name[NAME_LEN];
    if (read(conn, name, NAME_LEN) <= 0) {  // read() failed
      perror("start_accepting() : read() failed");
      pthread_exit(NULL);
    }

    // Look for existing client registration
    int index = -1;  // <- index of existing client (if found)
    for (int x = 0; x < MAX_NUM_CLI; x++) {
      if (strncmp(clients[x].name, name, NAME_LEN) == 0) {  // name matches
        index = x;
        break;
      }
    }

    // Check if client already registered in clients array
    if (index == -1) {  // client not found
      // Create placeholder for new client
      client_t client;

      // Set client name
      strncpy(client.name, name, NAME_LEN);

      // Initialize client connection placeholder
      client.conn = conn_t_new();

      // Set RX connection socket
      client.conn.rx = conn;

      // Find vacant client placeholder in array
      int vacant_index = -1;
      for (int x = 0; x < MAX_NUM_CLI; x++) {
        // Check client placeholder status
        if (client_t_stat(clients[x]) == 0) {  // client placeholder is vacant
          // Save new client in vacant position
          clients[x] = client;

          // Update vacant index
          vacant_index = x;

          // Stop looking for vacant clients
          break;
        }
      }

      // Check if no vacant placeholders found
      if (vacant_index == -1) {  // failed to add client
        fprintf(stderr,
                "start_accepting() : no vacant clients in array found.\n");
        pthread_exit(NULL);
      }
    }

    // Otherwise, client is already registered.
    // Set remaining connection socket
    else if (index < MAX_NUM_CLI) {
      // Check client connection placeholders.
      // Set the uninitialized one to conn.
      if (clients[index].conn.rx == -1)
        clients[index].conn.rx = conn;
      else if (clients[index].conn.tx == -1)
        clients[index].conn.tx = conn;
      else
        continue;  // nothing to do.

      // Client is ready //

      // Start routing client
      pthread_t thread_start_routing_client;
      if ((errno = pthread_create(&thread_start_routing_client, NULL,
                                  start_routing_client, &clients[index])) !=
          0) {  // pthread_create() failed
        perror("pthread_create() failed");
        pthread_exit(NULL);
      }

      // Detach thread
      if ((errno = pthread_detach(thread_start_routing_client)) !=
          0) {  // pthread_detach() failed
        perror("pthread_detach() failed");
        pthread_exit(NULL);
      }

      // Check if currently unit testing
      if (debug) break;
    }

    // Invalid index returned. Unknown error.
    else {
      fprintf(stderr,
              "registered client index value is invalid. unknown problem\n");
      pthread_exit(NULL);
    }

    // Client is ready
    continue;
  }
}

// Thread which routes messages for an individual client
static void *start_routing_client(void *params) {
  // Create placeholder for client parameter
  client_t client = *((client_t *)params);

  for (;;) {
    // Create placeholder for incoming message
    char name[NAME_LEN];
    char msg[MAX_MSG_LEN];
    int msg_len = 0;

    // Check if rx/tx sockets ready
    if (conn_t_stat(client.conn) == -1) {  // connection is not ready
      // Wait a second before trying again
      sleep(1);
      continue;
    }

    // Check if any packets waiting in queue
    if (!ipc_packet_waiting(packets, MAX_NUM_PACKETS)) {
      // Wait for request from client
      char incoming_data[MAX_PACKET_LEN];
      int bytes_read = -1;
      if ((bytes_read = read(client.conn.tx, incoming_data, MAX_PACKET_LEN)) <=
          0) {
        // Exactly 0 bytes read. Disconnect client.
        if (bytes_read == 0) {
          ipcd_printf("read 0 bytes from client [%.*s]. disconnecting...\n",
                      NAME_LEN, client.name);
          disconnect_client((client_t *)params);
          pthread_exit(NULL);
        }

        fprintf(stderr, "read() failed : start_routing_client() failed\n");

        // Disconnect client from network
        disconnect_client((client_t *)params);

        // Kill client router thread
        pthread_exit(NULL);
      }

      // Check length of incoming message
      if (bytes_read <= NAME_LEN) {  // name or message missing
        fprintf(stdout, "message received without name or message. SKIPPING\n");
        continue;
      }

      // Check if disconnect signal sent
      if (strncmp(msg, DISCONNECT_SIG, bytes_read) ==
          0) {  // disconnect signal sent
        // Close client
        client_t_close((client_t *)params);

        // Stop thread
        pthread_exit(NULL);
      }

      // Parse incoming data into packets
      ipc_packet_t next_packet;
      char overflow[MAX_PACKET_LEN];
      int overflow_len = -1;
      bool skip = false;
      for (int x = 0; x < MAX_NUM_PACKETS; x++) {
        overflow_len =
            ipc_packet_parse(incoming_data, bytes_read, &next_packet, overflow);

        // Catch parsing errors
        if (overflow_len < 0) {
          // Continue to next request
          fprintf(
              stderr,
              "ipc_packet_parse() failed for client %.*s. SKIPPING PACKET.\n",
              NAME_LEN, client.name);
          skip = true;
          break;
        }

        // Add parsed packet to queue
        ipc_packet_add(packets, MAX_NUM_PACKETS, next_packet);

        // Check for overflow
        if (overflow_len) {
          // Replace original data with overflow
          strncpy(incoming_data, overflow, overflow_len);
          bytes_read = overflow_len;
          continue;
        } else {
          break;
        }
      }

      if (skip) continue;
    }

    // Pop packet from queue
    ipc_packet_t packet = ipc_packet_pop(packets, MAX_NUM_PACKETS);

    // Check if packet blank
    if (!ipc_packet_blank(packet)) {
      // Export packet into message
      msg_len = ipc_packet_export(packet, name, msg, MAX_MSG_LEN);
    } else {
      // Continue to next request
      continue;
    }

    // Look for destination client in clients array
    for (int x = 0; x < MAX_NUM_CLI; x++) {
      // Check if destination name matches another client's name
      if (strncmp(clients[x].name, name, NAME_LEN) == 0) {  // name matches
        // Format outgoing packet
        char outgoing_msg[MAX_PACKET_LEN];
        int outgoing_len = sprintf(outgoing_msg, "%.*s <%.*s>", NAME_LEN,
                                   client.name, msg_len, msg);

        // Send message to destination client
        if (write(clients[x].conn.rx, outgoing_msg, outgoing_len) <
            outgoing_len) {  // write() failed
          perror("write() failed : start_routing_client() failed");
          pthread_exit(NULL);
        }

        // Traffic Debug
        if (LOG_TRAFFIC)
          log_traffic(client.name, clients[x].name, msg, msg_len);
      }
    }

// Only run contents of loop once if debugging
#ifdef DEBUG
    break;
#endif
  }
}

/**
 * Disconnect client from IPC network.
 *
 * Closes associated sockets and clears placeholder.
 */
void disconnect_client(client_t *client) {
  // Log disconnection
  ipcd_printf("Disconnected [%.*s] from IPC daemon.\n", NAME_LEN, client->name);

  // Close connections
  conn_t_close(&client->conn);

  // Replace client with vacant placeholder
  *client = client_t_new();

  // done
}

/////////////////////
//  Public Methods //
/////////////////////

// Initialize the IPC daemon
int ipcd_init() {
  // Initialize clients array
  for (int x = 0; x < MAX_NUM_CLI; x++) {
    clients[x] = client_t_new();
  }

  // Initialize listening socket for incoming clients
  if ((sock_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {  // socket() failed
    perror("socket() failed");
    return -1;
  }

  // Create placeholders for socket address
  const struct sockaddr_un address = {.sun_family = AF_UNIX,
                                      .sun_path = "./socket.socket"};
  const socklen_t address_len = sizeof(address);

  // Check if socket file missing
  if (access(address.sun_path, F_OK) != 0) {
    // Create file
    system("mkfifo socket.socket");
  }

  // Unlink socket
  if (unlink(address.sun_path) == -1) {  // unlink() failed
    perror("unlink() failed");
    return -1;
  }

  // Bind socket to fd at specified address
  if (bind(val(sock), (struct sockaddr *)&address, address_len) ==
      -1) {  // bind() failed
    perror("bind() failed");
    return -1;
  }

  // Put socket into listen mode
  if (listen(val(sock), MAX_NUM_CLI) == -1) {  // listen() failed
    perror("listen() failed");
    return -1;
  }

  // Start accepting clients
  pthread_t thread_start_accepting;
  if ((errno = pthread_create(&thread_start_accepting, NULL, start_accepting,
                              0)) != 0) {  // pthread_create() failed
    perror("pthread_create() failed");
    return -1;
  }

  // Detach thread
  if ((errno = pthread_detach(thread_start_accepting)) !=
      0) {  // pthread_detach() failed
    perror("pthread_detach() failed");
    return -1;
  }

  // done
  return 0;
}

// Shutdown the IPC daemon
int ipcd_close() {
  // Close incoming connection socket
  close(val(sock));

  // done
  return 0;
}

// Prints the list of clients
int ipcd_print_clients() {
  for (int x = 0; x < MAX_NUM_CLI; x++) {
    printf("Client #%d\n", x);
    printf("{\n");
    printf("\tname = %.3s\n", clients[x].name);
    printf("\tconn = {\n");
    printf("\t\t.rx = %d,\n", clients[x].conn.rx);
    printf("\t\t.tx = %d\n", clients[x].conn.tx);
    printf("\t}\n");
    printf("}\n\n");
  }

  // done
  return 0;
}

static void log_traffic(char src[NAME_LEN], char dest[NAME_LEN],
                        char msg[MAX_PACKET_LEN], size_t msg_len) {
  // Print log
  ipcd_printf("[%.*s] ===> [%.*s] (%.*s)\n", NAME_LEN, src, NAME_LEN, dest,
              msg_len, msg);
}

static void ipcd_printf(const char *msg, ...) {
  // Init variadic arg
  va_list ap;
  va_start(ap, msg);

  // Get current time
  time_t current_time = time(NULL);
  struct tm *lt = localtime(&current_time);

  // Print time
  fprintf(stdout, "(%.02d:%.02d:%.02d) ", lt->tm_hour, lt->tm_min, lt->tm_sec);

  // Print message
  vfprintf(stdout, msg, ap);
  // fprintf(stdout, "\n");

  // Cleanup
  va_end(ap);

  // done
}