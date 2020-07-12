/*
* ipcd.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*/

// Project headers
#include "ipc/ipcd.h"

// TODO: make ipcd part of the libipc 

///////////////////////
//  Local Variables  //
///////////////////////

// Placeholder for connected clients
static client_t clients[MAX_NUM_CLI];

// Placeholder for incoming connection socket
static int sock_ = -1;
static immut(int) sock = &sock_;

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
    // if((conn = accept(val(sock), NULL, NULL)) == -1) // accept() failed 
    // {
    //   perror("start_accepting() : accept() failed");
    //   pthread_exit(NULL);
    // }
    if((conn = accept4(val(sock), NULL, NULL, SOCK_NONBLOCK) == -1)) // accept() failed 
    {
      perror("start_accepting() : accept() failed");
      pthread_exit(NULL);
    }

    // Configure newly created socket 
    if(setsockopt(conn, SOL_SOCKET, SO_TYPE, SOCK_STREAM | SOCK_NONBLOCK, sizeof(SOCK_STREAM)))
    {

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
        break;
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
          // Save new client in vacant position
          clients[x] = client;

          // Update vacant index
          vacant_index = x;

          // Stop looking for vacant clients 
          break;
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
    // Set remaining connection socket
    else if(index < MAX_NUM_CLI)
    {
      // Check client connection placeholders.
      // Set the uninitialized one to conn.
      if(clients[index].conn.rx == -1) clients[index].conn.rx = conn;
      else if(clients[index].conn.tx == -1) clients[index].conn.tx = conn;
      else continue;  // nothing to do.
    }

    // Client is ready
    continue;
  }
}

// Thread which routes messages for an individual client 
static void * start_routing_client(void * params)
{
  // Create placeholder for client parameter 
  client_t client = *((client_t *) params);

  for(;;)
  {
    // Create placeholder for incoming message 
    char msg[MAX_MSG_LEN];
    
    // Wait for request from client 
    int bytes_read = -1;
    if((bytes_read = read(client.conn.tx, msg, MAX_MSG_LEN)) <= 0) // read() failed 
    {
      fprintf(stderr, "read() failed : start_routing_client() failed\n");
      pthread_exit(NULL);
    }

    // 
  }
}

// Thread which initializes routing threads for individual clients
static void * start_routing()
{
  // Create placeholder for client router threads 
  pthread_t client_routers[MAX_NUM_CLI];

  // Initialize client routers to -1
  for(int x = 0; x < MAX_NUM_CLI; x++) client_routers[x] = -1;

  for(;;)
  {
    // Parse through clients in clients array 
    for(int x = 0; x < MAX_NUM_CLI; x++) 
    {
      // Check if active client has an active router 
      if(client_t_stat(clients[x]) == 1 && client_routers[x] == -1) // active client is missing router 
      {
        // Start routing messages for client 
        if((errno = pthread_create(&client_routers[x], NULL, start_routing_client, &clients[x])) != 0) // pthread_create() failed 
        {
          perror("pthread_create() failed");
          pthread_exit(NULL);
        }
      }
    }

    // Delay before checking client array again 
    const struct timespec router_delay = {
      .tv_nsec = ROUTER_CHECK_DELAY
    };
    nanosleep(&router_delay, NULL);
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
  pthread_t thread_start_accepting;
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

// Start routing messages between clients 
int ipcd_start_routing()
{
  // Create placeholder for start routing thread 
  pthread_t thread_start_routing;

  // Create thread 
  if((errno = pthread_create(&thread_start_routing, NULL, start_routing, NULL)) != 0) // pthread_create() failed 
  {
    perror("pthread_create() failed");
    return -1;
  }

  // Detach thread 
  if((errno = pthread_detach(thread_start_routing)) != 0) // pthread_detach() failed 
  {
    perror("pthread_detach() failed");
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

// Prints the list of clients
int ipcd_print_clients()
{
  for(int x = 0; x < MAX_NUM_CLI; x++)
  {
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