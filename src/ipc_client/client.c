/*
* client.c
*
*   purpose: Uses the client API to connect to the IPC.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

// Standard C libraries
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

// Interrupt signal routine handler
void isr(int sig);

// Create callbacks for incoming messages
static void cb_wild(char *msg,size_t msg_len) {
  printf("\r[*] >> %.*s\n", msg_len, msg);
  printf(">> ");
  fflush(stdout);
}

static void cb_bob(char *msg, size_t msg_len) {
  printf("\r[bob] >> %.*s\n", msg_len, msg);
  printf(">> ");
  fflush(stdout);
}

static void cb_yan(char *msg, size_t msg_len) {
  printf("\r[yan] >> %.*s\n", msg_len, msg);
  printf(">> ");
  fflush(stdout);
}

static void *auto_refresh() {
  for(;;) {
    ipc_refresh();
    nanosleep(&READ_BLOCK_DELAY, NULL);
  }
}

int main(int argc, char* argv[]) {
  // Check argc
  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments\n Try: ./client <name> <read/write/async> (async is experimental)\n");
    return -1;
  }

  // Create placeholder for client name
  char* name = argv[1];
  char* rdwr = argv[2];
  
  // Copy read/write setting into formatted array 
  char rdwr_fmt[5];
  strcpy(rdwr_fmt, rdwr);

  // Format rdwr real quick 
  if(strcmp(rdwr, "read") == 0) {
    rdwr_fmt[4] = ' ';
  }

  // Check name length
  if (strlen(name) != 3) {  // name is not correct length
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Connect to IPC system
  if (ipc_connect(name) == -1) {  // ipc_connect() failed
    fprintf(stderr, "Failed to connect to the ipc\n");
    return -1;
  }

  // Welcome prompt 
  printf("\
  ################################################\n\
  #           IPC client example program         #\n\
  ################################################\n\
  #                  Settings                    #\n\
  #  Name: %.3s                                   #\n\
  #  Mode: %.5s                                 #\n\
  ################################################\n\
  #                                              #\n\
  #     Press [CTRL + C] at any time to quit     #\n\
  #                                              #\n\
  ################################################\n\n",\
  name, rdwr_fmt);

  // Check if client reading or writing
  if (strcmp(rdwr, "read") == 0) {  // reading
    printf("[waiting for incoming messages...]\n\n");

    for (;;) {
      // Create placeholder for incoming message
      char src[NAME_LEN+2];
      char msg[MAX_MSG_LEN];

      // Ask user for source filter 
      printf("Please enter the desired source filter (* for wildcard): ");
      fgets(src, NAME_LEN+2, stdin);

      // Read data
      int bytes_read = -1;
      if ((bytes_read = ipc_recv(src, msg, MAX_MSG_LEN)) == -1) {  // ipc_recv() failed
        fprintf(stderr, "ipc_recv() failed\n");
        return -1;
      }

      // Print data
      printf("message received: %s\n", msg);
    }
  }

  else if (strcmp(rdwr, "write") == 0) {  // writing
    for (;;) {
      // Create placeholders for message to send
      char dest[NAME_LEN + 2];
      char msg[MAX_MSG_LEN + 2];

      // Ask user for message and destination
      printf("Enter destination's name: ");
      fgets(dest, NAME_LEN + 2, stdin);

      // Ask user for message
      printf("Enter message: ");
      fgets(msg, MAX_MSG_LEN + 2, stdin);

      // Check if both entries are the [ENTER] key 
      if(strncmp(dest, "\n", 1) == 0 && strncmp(msg, "\n", 1) == 0) {
        // quit
        break;
      }

      // Check if message 'quit'
      if (strcmp(msg, "quit") == 0) {  // user is asking to quit
        break;
      }

      // Send message to other client
      if (ipc_send(dest, msg, strlen(msg)) == -1) {  // ipc_send() failed
        fprintf(stderr, "ipc_send() failed\n");
        return -1;
      }
    }
  }

  else if (strcmp(rdwr, "async") == 0) {  // async communication
    // Create placeholders for messages
    char msg_send[MAX_MSG_LEN + 2];
    char src[NAME_LEN + 2];

    // Create dibs 
    if(ipc_qrecv("bob", cb_bob) != 0) {
      fprintf(stderr, "ipc_qrecv failed : ");
      return -1;
    }

    if(ipc_qrecv("yan", cb_yan) != 0) {
      fprintf(stderr, "ipc_qrecv failed : ");
      return -1;
      }

    if(ipc_qrecv("*", cb_wild) != 0) {
      fprintf(stderr, "ipc_qrecv failed : ");
          return -1;
        }

    // Start refreshing for incoming messages 
    pthread_t thread_auto_refresh;
    if(pthread_create(&thread_auto_refresh, NULL, auto_refresh, NULL) < 0) {
      perror("pthread_create() failed");
      return -1;
      }

    // Message send loop
    for(;;) {
      // Create placeholders for user defined message
      char input[MAX_MSG_LEN + 2];
      char dest[NAME_LEN + 2];
      char msg[MAX_MSG_LEN + 2];

      // Ask user for message to send 
      printf(">> ");
      fgets(input, MAX_MSG_LEN + 2, stdin);

      // Separate dest name from msg 
      strncpy(dest, input, NAME_LEN);
      strncpy(msg, &input[NAME_LEN+1], MAX_MSG_LEN - (NAME_LEN+1));

      // Send message 
      if(ipc_send(dest, msg, strlen(msg)) != 0) {
        fprintf(stderr, "ipc_send() failed\n");
        return -1;
      }
    }
  }

  else if(strcmp(rdwr, "dibs") == 0) {  // async communication with dibs
    // TODO: make dibs version for client to test dibs feature
    }

  else {  // bad keyword
    fprintf(stderr, "invalid setting. try ./client <name> <read/write>\n");
    return -1;
  }

  printf("presss [ENTER] to quit\n");
  fgetc(stdin);

  // Close IPC
  ipc_disconnect();

  // done
  return 0;
}

// Interrupt signal routine 
void isr(int sig) {
  switch(sig) {
    case SIGINT:
      ipc_disconnect();
      exit(0);
      break;
  }
}

void callback(char* msg, size_t msg_len) {
  printf("\nI AM CALLBACK\n");
}