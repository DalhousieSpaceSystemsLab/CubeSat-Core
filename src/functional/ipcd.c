// trying to do some functional C style programming...

// IPC daemon header 
#include "ipc/ipcd.h"

///////////////////////
//  Private methods  //
///////////////////////

// Returns pointer to unique client array
static client_t * clients_(size_t n)
{
  // Create new clients array 
  client_t * const clients = (client_t *) malloc(n * sizeof(client_t));

  // done 
  return clients;
}

// Returns incoming connection socket
static int socket_()
{
  // Create socket placeholder 
  static int sock = -1;

  // Check if placeholder uninitialized
  if(sock == -1) // socket is uninitialized
  {
    return (sock = socket(AF_UNIX, SOCK_STREAM, 0));
  }

  // return socket
  return sock;
}

// Incoming connection socket address
static const struct sockaddr_un address = {
  .sun_family = AF_UNIX,
  .sun_path   = "./socket.socket"
};

// Incoming connection socket address length
static const socklen_t address_len = sizeof(address);

// Modified accept() method which only accepts the next client if mut = 1.
// Otherwise, it returns the previously initialized value.
static int accept_(int mut)
{
  // Create connection 
  static int conn = -1;

  // Check if mutable is enabled 
  if(mut == 1)
  {
    // Update conn 
    conn = accept(socket_(), (struct sockaddr *) &address, &address_len);
  }

  // done
  return conn;
}

// Modified read() method which only reads more data if mut = 1.
// Otherwise, it returns the previously read value 
int read_(int sockfd, void * buf, int buf_len, int mut)
{
  // Create placeholder for read data 
  static char data[MAX_MSG_LEN];

  // Check if mutable
  if(mut == 1)
  {
    // Read more data 
    read(socket_(), data, MAX_MSG_LEN);
  }
}

// Accept incoming clients via the incoming connection socket 
static void * start_accepting()
{
  for(;;)
  {
    // Accept new client 
    if(accept_(1) == -1) // accept_() failed
    {
      perror("accept() failed");
      fprintf(stderr, "start_accepting() failed\n");
      pthread_exit(NULL);
    }

    // Create placeholder for client name 
    char name[3];

    // Get client name 
    if(read(accept_(0), name, 3) <= 0) // read() failed 
    {
      
    }
  }
}

//////////////////////
//  Public methods  //
//////////////////////

// Initialize the IPC daemon
int ipcd_init()
{
  // Unlink socket 
  if(unlink(address.sun_path) == -1) // unlink() failed 
  {
    perror("unlink() failed");
    return -1;
  }

  // Bind socket to address 
  if(bind(socket_(), (struct sockaddr *) &address, address_len) == -1) // bind() failed 
  {
    perror("bind() failed");
    return -1;
  } 

  // Put socket into listening mode 
  if(listen(socket_(), MAX_NUM_CLI) == -1) // listen() failed 
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
  // Create placeholder for thread ID
  pthread_t thread;

  // Create start_accepting thread 
  if(pthread_create(&thread, NULL, start_accepting, NULL) != 0) // pthread_create() failed
  {
    fprintf(stderr, "pthread_create() failed : ");
    return -1;
  }
  
  // Detach thread
  if(pthread_detach(thread) != 0) // pthread_detach() failed 
  {
    fprintf(stderr, "pthread_detach() failed : ");
    return -1;
  }
  
  // done
  return 0;
}

// temporary main function
int main()
{
  // test 
  ipcd_init();

  // done
  return 0;
}