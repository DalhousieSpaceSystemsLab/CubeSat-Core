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
static int                sock = -1;

// Placeholder for incoming connection socket address
static struct sockaddr_un address;
static        socklen_t   address_len = -1;

//////////////
//  Methods //
//////////////

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
int ipcd_accept()
{
  // Accept incoming connection 
  // if(accept(sock, ))

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

