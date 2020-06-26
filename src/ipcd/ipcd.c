/*
* ipcd.c
*
*   purpose: act a middleman between processes to complete communication requests
*   author: alex amellal
*
*/

// Project headers
#include "ipc/ipcd.h"

////////////////////////
//  Global Variables  //
////////////////////////

// Placeholder for connected clients
static client_t * clients = NULL;

// Placeholder for incoming connection socket
static int sock = -1;

// Placeholder for incoming connection socket address
static struct sockaddr_un address;
static        socklen_t   address_len = -1;

// Placeholders for threads 
pthread_t thread_start_accepting;

//////////////////////
//  Private Methods //
//////////////////////

// Start accepting incoming client connections
static void * start_accepting(void * params);

// Returns the index of the matching client if found.
// If no match is found, returns -1.
static int get_client_index(char name[3]);

/////////////////////
//  Public Methods //
/////////////////////

// Initialize the IPC daemon
int ipcd_init()
{
  // Allocate memory for clients
  if((clients = (client_t *) malloc(MAX_NUM_CLI * sizeof(client_t))) == NULL) // malloc() failed
  {
    fprintf(stderr, "malloc() failed : ");
    return -1;
  }

  // Initialize listening socket for incoming clients
  if(socket(sock, AF_UNIX, SOCK_STREAM) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Initialize socket address
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, "./socket.socket");

  // Set address length
  address_len = sizeof(address);

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
  free(clients);

  // Close incoming connection socket
  close(sock);

  // done
  return 0;
}

//////////////
//  Threads //
//////////////

static void * start_accepting(void * params)
{
  for(;;)
  {
    // Accept new client 
    if(accept(sock, (struct sockaddr *) &address, &address_len) == -1) // accept() failed
    {
      perror("accept() failed");
      pthread_exit(NULL);
    }

    // Create placeholder for client name 
    char name[3];

    // Read client name 
    if(read(sock, name, 3) < 3) // read() failed
    {
      fprintf(stderr, "read() failed : ");
      return -1;
    }

    // Check if client already registered

  }
}

// Returns the index of the matching client if found.
// If no match is found, returns -1.
static int get_client_index(char name[3])
{
  // Create placeholder for matching client index
  int index = -1;

  // Parse through clients in client array 
  for(int x = 0; x < MAX_NUM_CLI; x++) 
  {
    if(strcmp(name, clients[x].name) == 0) 
    {
      if(clients[x].conn.rx != -1 && clients[x].conn.tx != -1)
      {
      index = x;
      }
    }

// Adds client to first available free slot in specified array
// Returns newly created client's index in the client array 
// Returns -1 if client already exists or if no free space available.
static int add_client(char name[3])
{
  // Check if client already registered
  if(get_client_index(name) != -1) // client exists
  {
    fprintf(stderr, "client already exists : ");
    return -1;
  }

  // Get index of available client slot 
  int index = -1;
  if((index = get_free_client_index()) == -1) // no free slots available 
  {
    fprintf(stderr, "no free client slots available : ");
    return -1;
  }

  // Create new client 
  clients[index] = client_t_new();
  
  // Set client name 
  strncpy(clients[index].name, name, 3);

  // TODO : finish add_client function AND start_accepting thread

  // done
  return 0;
}