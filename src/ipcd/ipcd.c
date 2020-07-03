/*
* ipcd.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*/

// Project headers
#include "ipc/ipcd.h"

///////////////////////
//  Local Variables  //
///////////////////////

// Placeholder for connected clients
static client_t clients[MAX_NUM_CLI];

// Placeholder for incoming connection socket
static int sock_ = -1;
static immut(int) sock = &sock_;

// Placeholders for threads 
static pthread_t thread_start_accepting;

//////////////////////
//  Private Methods //
//////////////////////

// Thread which processes incoming client connections
static void * start_accepting()
{
  for(;;)
  {
    // Create placeholder for client connection
    int conn = -1;

    // Accept new client 
    if((conn = accept(val(sock), NULL, NULL)) == -1) // accept() failed 
    {
      perror("start_accepting() : accept() failed");
      pthread_exit(NULL);
    }

    // Get client name 
    char name[NAME_LEN];
    if(read(conn, name, NAME_LEN) <= 0) // read() failed 
    {
      perror("start_accepting() : read() failed");
      pthread_exit(NULL);
    }

    // Look for existing client registration 
    int index = -1; // <- index of existing client (if found)
    for(int x = 0; x < MAX_NUM_CLI; x++)
    {
      if(strncmp(clients[x].name, name, NAME_LEN) == 0) // name matches
      {
        index = x;
      }
    }

    // Check if client already registered in clients array 
    if(index == -1) // client not found 
    {
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
      for(int x = 0; x < MAX_NUM_CLI; x++)
      {
        // Check client placeholder status 
        if(client_t_stat(clients[x]) == 0) // client placeholder is vacant 
        {
          clients[x] = client;
          vacant_index = x;
        }
      }

      // Check if no vacant placeholders found 
      if(vacant_index == -1) // failed to add client 
      {
        fprintf(stderr, "start_accepting() : no vacant clients in array found.\n");
        pthread_exit(NULL);
      }
    }

    // Otherwise, client is already registered.
    // Set remaining tx connection socket
    else 
    {
      clients[index];
    }
  }
}

/////////////////////
//  Public Methods //
/////////////////////

// Initialize the IPC daemon
int ipcd_init()
{
  // Initialize clients array 
  for(int x = 0; x < MAX_NUM_CLI; x++) 
  {
    clients[x] = client_t_new();
  }

  // Initialize listening socket for incoming clients
  if((sock_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Create placeholders for socket address 
  const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket"
  };
  const socklen_t address_len = sizeof(address);

  // Unlink socket 
  if(unlink(address.sun_path) == -1) // unlink() failed 
  {
    perror("unlink() failed");
    return -1;
  }

  // Bind socket to fd at specified address
  if(bind(val(sock), (struct sockaddr *) &address, address_len) == -1) // bind() failed 
  {
    perror("bind() failed");
    return -1;
  }

  // Put socket into listen mode 
  if(listen(val(sock), MAX_NUM_CLI) == -1) // listen() failed 
  {
    perror("listen() failed");
    return -1;
  }

  // done
  return 0;
}

// Start accepting incoming client connections
int ipcd_start_accepting()
{
  // Create start_accepting thread 
  if(pthread_create(&thread_start_accepting, NULL, start_accepting, NULL) != 0) // pthread_create() failed
  {
    fprintf(stderr, "pthread_create() failed : ");
    return -1;
  }

  // Detach thread 
  if(pthread_detach(thread_start_accepting) != 0) // pthread_detach() failed 
  {
    fprintf(stderr, "pthread_detach() failed : ");
    return -1;
  }

  // done 
  return 0;
}

// Shutdown the IPC daemon
int ipcd_close()
{
  // Close incoming connection socket
  close(val(sock));

  // done
  return 0;
}