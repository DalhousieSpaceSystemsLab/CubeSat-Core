/*
* client_old.c
*
*   purpose: connects to the ipc with a specific name and the ability to send messages to other clients
*   author: alex amellal
*
*/

// Standard C Libraries
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Create socket placeholder
static int sock;

// Threads
static void * start_listening();
static void * start_sending();

int main(int argc, char * argv[])
{
  // Check argc
  if(argc != 2)
  {
    fprintf(stderr, "Invalid number of arguments\n Try: ./client <name>\n");
    return -1;
  }

  // Create placeholder for client name
  char * name = argv[1];

  // Check name length
  if(strlen(name) > 3) // name is too long
  {
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Create placeholder for socket address
  const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket"
  };
  socklen_t address_len = sizeof(address);

  // Initiate socket
  if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) // socket() failed
  {
    perror("socket() failed");
    return -1;
  }

  // Connect to host
  if(connect(sock, (struct sockaddr *) &address, address_len) == -1) // connect() failed
  {
    perror("connect() failed");
    return -1;
  }

  // Send name to host
  if(write(sock, name, strlen(name)) < strlen(name)) // write() failed
  {
    perror("write() failed");
    return -1;
  }

  // Start listener and sender threads
  pthread_t listener, sender;
  pthread_create(&listener, NULL, start_listening, NULL);
  pthread_create(&sender, NULL, start_sending, NULL);

  // Wait for sending thread to finish
  pthread_join(sender, NULL);

  // Close socket connection
  close(sock);

  // done
  return 0;
}

static void * start_listening()
{
  // Create placeholder for incoming messages
  char msg[128];
  int msg_len = -1;

  // Await for incoming messages
  while((msg_len = read(sock, msg, 128)) > 0) // until a 0 length message is read or error is encountered
  {
    // Null-terminate message
    msg[msg_len] = '\0';

    // Check if message is 'exit'
    if(strcmp(msg, "exit") == 0) break;

    // Prompt user with incoming message
    printf("\rIncoming message: %s\nEnter message for IPC: ", msg);
    fflush(stdout);
  }

  // done
  pthread_exit(NULL);
}

static void * start_sending()
{
  for(;;)
  {
    // Create placeholder for request
    char req[128];

    // Get input from user
    printf("\rEnter request for IPC: ");
    fgets(req, 128, stdin);

    // Convert newline character to null termination
    for(int x = 0; x < strlen(req); x++) if(req[x] == '\n') req[x] = '\0';

    // Send request to IPC
    if(write(sock, req, strlen(req)) < strlen(req)) // write() failed
    {
      perror("write() failed");
      pthread_exit(NULL);
    }

    // Check if request was 'exit'
    if(strcmp(req, "exit") == 0) pthread_exit(NULL);
  }
}
