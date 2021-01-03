/*
* client_t.c
*
*   purpose:  defines standardized methods for the client type.
*   author:   alex amellal
*
*/

// Client type
#include "ipc/client_t.h"

// Returns initialized client_t
client_t client_t_new() {
  // Create placeholder for initialized client
  client_t client;

  // Initialize client connection placeholder
  client.conn = conn_t_new();

  // Initialize client name
  memset(client.name, 0, NAME_LEN);

  // done
  return client;
}

// Returns 0 if client is blank, 1 if used
int client_t_stat(client_t client) {
  // Check name
  for (int x = 0; x < NAME_LEN; x++) {
    // Iterate through name characters to ensure they're blank
    if (client.name[x] != 0) {  // non-blank character found
      // Declare used client
      return 1;
    }
  }

  // done
  return 0;
}

// Closes conn member of client_t
int client_t_close(client_t* client) {
  // Close client connections
  conn_t_close(&client->conn);

  // Wipe name entry clean
  memset(client->name, 0, 3);

  // done
  return 0;
}