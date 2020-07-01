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
static int sock = -1;

// Placeholder for incoming connection socket address
static struct sockaddr_un address;
static        socklen_t   address_len = -1;

// Placeholders for threads 
static pthread_t thread_start_accepting;

//////////////////////
//  Private Methods //
//////////////////////

// static void * start_accepting(void * params)
// {
//   for(;;)
//   {
//     // Accept new client 
//     int conn = -1;
//     if((conn = accept(sock, (struct sockaddr *) &address, &address_len)) == -1) // accept() failed
//     {
//       perror("accept() failed");
//       fprintf(stderr, "accept() failed : start_accepting() failed\n");
//       pthread_exit(NULL);
//     }

//     // Create placeholder for client name 
//     char name[3];

//     // Read client name 
//     if(read(sock, name, 3) < 3) // read() failed
//     {
//       perror("start_accepting() : read() failed");
//       pthread_exit(NULL);
//     }

//     // Add client 
//     int index = -1;
//     if((index = register_client(name)) == -1) // register_client() failed 
//     {
//       fprintf(stderr, "register_client() failed : start_accepting() failed\n");
//       pthread_exit(NULL);
//     }

//     // Check client connection status
//     if(conn_t_stat(clients[index]) == -1) // conn is uninitialized
//     {
//       // Check if RX socket uninitialized 
//       if(clients[index].conn.rx == -1)
//       {
//         clients[index].conn.rx = conn;
//       } 
      
//       // Check if TX socket uninitialized
//       else if(clients[index].conn.tx == -1)
//       {
//         clients[index].conn.tx = conn;
//       }
//     }
//   }
// }

// Thread which processes incoming client connections
static void * start_accepting()
{
  for(;;)
  {
    // Accept new client 
    int conn = -1;
    if((conn = accept(sock, (struct sockaddr *) &address, &address_len)) == -1) // accept() failed 
    {
      perror("start_accepting() : accept() failed");
      pthread_exit(NULL);
    }

    // Get client name 
    char name[NAME_LEN];
    if(read(sock, name, NAME_LEN) <= 0) // read() failed 
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

// Returns the index of the matching client if found.
// If no match is found, returns -1.
static int get_client_index(char name[NAME_LEN])
{
  // Create placeholder for matching client index
  int index = -1;

  // Parse through clients in client array 
  for(int x = 0; x < MAX_NUM_CLI; x++) 
  {
    // Check if client exists
    if(strcmp(clients[x].name, name) == 0) // name matches 
    {
      index = x;
    }
  }
    
  // done
  return index;
}

// Returns first free client slot in client array.
// If no slots are available, returns -1.
int get_free_client_index()
{
  // Create placeholder for free client index 
  int index = -1;

  // Parse through clients index 
  for(int x = 0; x < MAX_NUM_CLI; x++) 
  {
    // Check for vacant client
    if(client_t_stat(clients[x]) == -1) // free client found 
    {
      index = x;
    }
  }

  // done
  return index;
}

// Registers client to first available free slot in specified array
// Returns newly created client's index in the client array 
// Returns index of pre-existing client if name matches
// Returns -1 if no free space available.
static int register_client(char name[NAME_LEN])
{
  // Check if client already registered
  int index = -1;
  if((index = get_client_index(name)) != -1) // client exists
  {
    return index;
  }

  // Get index of available client slot 
  if((index = get_free_client_index()) == -1) // no free slots available 
  {
    fprintf(stderr, "no free client slots available : ");
    return -1;
  }

  // Create new client 
  clients[index] = client_t_new();
  
  // Set client name 
  strncpy(clients[index].name, name, NAME_LEN);

  // done 
  return index;
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
  if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Initialize socket address
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, "./socket.socket");

  // Set address length
  address_len = sizeof(address);

  // Unlink socket 
  if(unlink(address.sun_path) == -1) // unlink() failed 
  {
    perror("unlink() failed");
    return -1;
  }

  // Bind socket to fd at specified address
  if(bind(sock, (struct sockaddr *) &address, address_len) == -1) // bind() failed 
  {
    perror("bind() failed");
    return -1;
  }

  // Put socket into listen mode 
  if(listen(sock, MAX_NUM_CLI) == -1) // listen() failed 
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
  // Free memory for clients
  // free(clients);

  // Close incoming connection socket
  close(sock);

  // done
  return 0;
}