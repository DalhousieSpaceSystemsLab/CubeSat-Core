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
client_t client_t_new()
{
  // Create placeholder for initialized client
  client_t client = {
    .conn = {
      .rx = -1,
      .tx = -1
    }
  };

  // done
  return client;
}

// Closes conn member of client_t
int client_t_close(client_t * client)
{
  // Check if connection never opened. Otherwise, close.
  if(client->conn.rx != -1) close(client->conn.rx);
  if(client->conn.tx != -1) close(client->conn.tx);

  // done
  return 0;
}