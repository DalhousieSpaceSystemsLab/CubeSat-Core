/*
* client_handler.c
*
*   purpose:  handle client connections to the IPC. intended to be used as a thread.
*   author:   alex amellal
*
*   request formatting: <destination client name> <message>
*/

#include "ipc/client_handler.h"

// Clients array placeholder
static client_t * clients = NULL;

// Sets clients array pointer
void set_clients(client_t * clients_)
{
  // Set local clients to clients parameter
  clients = clients_;
}

// Returns index of next available client placeholder.
// Returns -1 if no client placeholders available.
int get_free_client()
{
  // Ensure clients array is not null
  if(clients == NULL)
  {
    fprintf(stderr, "clients array placeholder for client handler not initialized. refusing to proceed.\n");
    return -1;
  }

  // Parse through clients
  for(int x = 0; x < MAX_NUM_CLI; x++)
  {
    if(clients[x].conn.rx == -1 && clients[x].conn.tx == -1) return x;
  }

  // done, nothing found
  return -1;
}

// Processes client requests
void * handle_client_requests(void * client_)
{
  // Check if clients array placeholders are initialized
  if(clients == NULL)
  {
    fprintf(stderr, "clients array placeholder for client handler not initialized. refusing to proceed.\n");
    pthread_exit(NULL);
  }

  // Create placeholder for clients pointer
  client_t * client = (client_t *) client_;

  // Create placeholders for client request
  char  req[MAX_MSG_LEN];
  int   req_len;
  char  dest_name[3];
  char  message[MAX_MSG_LEN];

  // Handle incoming requests from client
  while((req_len = read(client->conn, req, MAX_MSG_LEN)) > 0)
  {
    // Check if request is at least <3 char> + space + <1 char> = 5 in length
    if(req_len < 4) // request is too short
    {
      fprintf(stderr, "client %s made a request that was too short. ignoring.\n", client->name);
      continue;
    }

    // Add null termination character to request
    req[req_len] = '\0';

    // Check if client requesting to exit
    if(strcmp(req, "exit") == 0) break;

    // Extract destination client name from request
    strncpy(dest_name, req, 3);

    // Create placeholder for destination client index in clients array
    int dest_client_index = -1;

    // Identify destination client within clients array
    for(int x = 0; x < MAX_NUM_CLI; x++)
    {
      // Check if connection active
      if(clients[x].conn == -1) continue; // skip

      // Check if name matches
      if(strncmp(clients[x].name, dest_name, 3) == 0) // name matches
      {
        dest_client_index = x;
        break;
      }
    }

    // Check if client found
    if(dest_client_index == -1) // client not found
    {
      // Skip request
      continue;
    }

    // Front message with request sender
    sprintf(message, "%s ", client->name);

    // Extract message from request
    for(int x = 4; x < strlen(req); x++) message[x] = req[x];

    // Add null termination character to message
    message[strlen(req)] = '\0';

    // Send message to destination client
    int bytes_written = -1;
    if((bytes_written = write(clients[dest_client_index].conn, message, strlen(message))) < (int) strlen(message)) // write() failed
    {
      perror("write() failed");
      fprintf(stderr, "wrote %d/%.0lu bytes from clients %.3s to %.3s. skipping to next client request.\n", bytes_written, strlen(message), client->name, clients[dest_client_index].name);
      continue;
    }

    // Quit after one iteration if currrently unit testing
    #if UNIT_TESTING
      break;
    #endif
  }

  // Close connection to client
  close(client->conn);

  // Free client placeholder
  client->conn = -1;

  // done
}
