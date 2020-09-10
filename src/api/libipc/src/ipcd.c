/**
* ipcd.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*   TODO:
    - Key-value pair sending 
    - C++ wrappers 
    - Queue multiple messages 
    - Message source filters + dibs on incoming messages?
*
*/

// Project headers
#include "ipcd.h"

///////////////////////
//  Local Variables  //
///////////////////////

// Placeholder for connected clients
static client_t clients[MAX_NUM_CLI];

// Placeholder for incoming connection socket
static int sock_       = -1;
static immut(int) sock = &sock_;

//////////////////////
//  Static Methods //
//////////////////////

static void* start_accepting(void *debug);
static void* start_routing_client(void *params);
static void disconnect_client(client_t* client);

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
        fprintf(stderr, "start_accepting() : no vacant clients in array found.\n");
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
      if ((errno = pthread_create(&thread_start_routing_client, NULL, start_routing_client, &clients[index])) != 0) {  // pthread_create() failed
        perror("pthread_create() failed");
        pthread_exit(NULL);
      }

      // Detach thread
      if ((errno = pthread_detach(thread_start_routing_client)) != 0) {  // pthread_detach() failed
        perror("pthread_detach() failed");
        pthread_exit(NULL);
      }

      // Check if currently unit testing
      if (debug) break;
    }

    // Invalid index returned. Unknown error.
    else {
      fprintf(stderr, "registered client index value is invalid. unknown problem\n");
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
    char msg[MAX_MSG_LEN];

    // Check if rx/tx sockets ready
    if (conn_t_stat(client.conn) == -1) {  // connection is not ready
      // Wait a second before trying again
      sleep(1);
      continue;
    }

    // Wait for request from client
    int bytes_read = -1;
    if ((bytes_read = read(client.conn.tx, msg, MAX_MSG_LEN)) <= 0) {  // read() failed
      if (bytes_read == 0) fprintf(stderr, "actually read 0 bytes\n");
      fprintf(stderr, "read() failed : start_routing_client() failed\n");
      
      // Disconnect client from network
      disconnect_client((client_t *) params);

      // Kill client router thread 
      pthread_exit(NULL);
    }

    // Check length of incoming message
    if (bytes_read <= NAME_LEN) {  // name or message missing
      fprintf(stdout, "message received without name or message. SKIPPING\n");
      continue;
    }

    // Check if disconnect signal sent
    if (strncmp(msg, DISCONNECT_SIG, bytes_read) == 0) {  // disconnect signal sent
      // Close client
      client_t_close((client_t *)params);

      // Stop thread
      pthread_exit(NULL);
    }

    // Create placeholder for destination name
    char name[NAME_LEN];

    // Extract destination name from message
    for (int x = 0; x < NAME_LEN; x++) {
      name[x] = msg[x];
    }

    // Look for destination client in clients array
    for (int x = 0; x < MAX_NUM_CLI; x++) {
      // Check if destination name matches another client's name
      if (strncmp(clients[x].name, name, 3) == 0) {  // name matches
        // Create placeholder for formatted message
        char fmt_msg[MAX_MSG_LEN];

        // Copy source client name into formatted message
        strncpy(fmt_msg, client.name, 3);

        // Add space between source client name and message
        fmt_msg[NAME_LEN] = ' ';

        // Copy rest of message into formatted message
        for (int y = NAME_LEN + 1; y < bytes_read; y++) fmt_msg[y] = msg[y];

        // Calculate formatted message length
        int fmt_msg_len = bytes_read;

        // Send message to destination client
        if (write(clients[x].conn.rx, fmt_msg, fmt_msg_len) < fmt_msg_len) {  // write() failed
          perror("write() failed : start_routing_client() failed");
          pthread_exit(NULL);
        }
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
void disconnect_client(client_t* client) {
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
  const struct sockaddr_un address = {
      .sun_family = AF_UNIX,
      .sun_path   = "./socket.socket"};
  const socklen_t address_len = sizeof(address);

  // Unlink socket
  if (unlink(address.sun_path) == -1) {  // unlink() failed
    perror("unlink() failed");
    return -1;
  }

  // Bind socket to fd at specified address
  if (bind(val(sock), (struct sockaddr *)&address, address_len) == -1) {  // bind() failed
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
  if ((errno = pthread_create(&thread_start_accepting, NULL, start_accepting, 0)) != 0) {  // pthread_create() failed
    perror("pthread_create() failed");
    return -1;
  }

  // Detach thread
  if ((errno = pthread_detach(thread_start_accepting)) != 0) {  // pthread_detach() failed
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