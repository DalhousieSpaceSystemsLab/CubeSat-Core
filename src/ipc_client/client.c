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

// Callbacks for async communication 
static void cb_read(char *msg, size_t msg_len, void* data);

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
      // printf("Please enter the desired source filter (* for wildcard): ");
      // fgets(src, NAME_LEN+2, stdin);

      // Read data
      int bytes_read = -1;
      if ((bytes_read = ipc_recv("*", msg, MAX_MSG_LEN)) == -1) {  // ipc_recv() failed
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
    // Create placeholder for dibs 
    char dibs[MAX_NUM_DIBS][NAME_LEN + 2];
    size_t dibs_len = 0;
    
    // Get message dibs names from client
    for(int x = 0; x < MAX_NUM_DIBS; x++) {
      // Ask user to enter dib
      printf("Enter a message source you'd like dibs on ([ENTER] when done): ");
      fgets(dibs[x], NAME_LEN + 2, stdin);

      // Check if user pressed enter 
      if(dibs[x][0] == '\n') break;

      // Increment dibs counter 
      dibs_len++;
    }

    // Create message dibs in IPC 
    for(int x = 0; x < dibs_len; x++) {
      // Check if ipc_qrecv failed
      if(ipc_qrecv(dibs[x], cb_read, NULL) != 0) {
        fprintf(stderr, "ipc_qrecv() failed\n");
        return -1;
      }
    }

    // Loop on demand. Give user choice between refresh and send msg. //

    // Prompt instructions 
    printf("### Message Loop ###\n\
    \tEnter [r] to refresh incoming messages.\n\
    \tEnter [s] to send a message\n\
    \tPress [ENTER] to quit\n\n");
    
    for(;;) {
      // Create placeholder for input 
      char ans[1 + 2];

      // Prompt user for input 
      printf("> ");
      fflush(stdout);

      // Get input 
      fgets(ans, 1 + 2, stdin);

      // Check if enter, refresh or send
      if(ans[0] == '\n') {
        // Quit
        break;
      } else if(ans[0] == 'r') {
        // Refresh 
        if(ipc_refresh() != 0) {
          fprintf(stderr, "ipc_refresh() failed\n");
          return -1;
        }
      } else if(ans[0] == 's') {
        // Create placeholder for message 
        char msg[MAX_MSG_LEN + 2];
        
        // Get message from user 
        printf(">> ");
        fgets(msg, MAX_MSG_LEN + 2, stdin);

        // Separate name from msg 
        char name[NAME_LEN];
        char msg_nameless[MAX_MSG_LEN];
        strncpy(name, msg, NAME_LEN);
        strncpy(msg_nameless, &msg[NAME_LEN+1], MAX_MSG_LEN - (NAME_LEN+1));

        // Send message 
        if(ipc_send(name, msg_nameless, strlen(msg_nameless)) != 0) {
          fprintf(stderr, "ipc_send() failed\n");
          return -1;
        }
      } else {
        // Not a recognized option, skip 
        continue;
      }
    }
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

// Callback for async communication 
static void cb_read(char *msg, size_t msg_len, void* data) {
  // Print message 
  printf("Incoming message: %.*s\n", msg_len, msg);
}