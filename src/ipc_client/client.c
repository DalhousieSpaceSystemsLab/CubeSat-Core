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

// Test callback for ipc_qrecv
void callback(char *, size_t);

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

    // Queue up a bunch of ipc_qrecv
    for (;;) {
      // Prompt user to queue ipc_qrecv request 
      printf("[?] Please enter a source filter (* for wildcard or [ENTER] to go to next step): ");
      fgets(src, NAME_LEN + 2, stdin);

      // Check to see if user pressed enter right away 
      if(strncmp(src, "\n", 1) == 0) {
        break;
      }

      // Run ipc_qrecv
      if(ipc_qrecv(src, callback) != 0) {
          fprintf(stderr, "ipc_qrecv() failed\n");
          return -1;
        }
      }

    // Keep refreshing 
    for(;;) {
      // Prompt user to enter refresh source filter 
      printf("[?] Please enter a source filter for ipc_refresh (* for wildcard or [ENTER] to quit): ");
      fgets(src, NAME_LEN + 2, stdin);

      // Check if user pressed enter right away 
      if(strncmp(src, "\n", 1) == 0) {
        break;
      }

      // Run ipc_refresh_src 
      if(ipc_refresh_src(src) != 0) {
        fprintf(stderr, "ipc_refresh_src() failed\n");
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