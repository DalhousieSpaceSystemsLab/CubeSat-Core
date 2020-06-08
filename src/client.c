/*
* client.c
*
*   purpose: Uses the client API to connect to the IPC.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

// Standard C Libraries
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void start_sending();
void start_recving();

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

  // Initialize IPC client interface
  if(ipc_init(name) == -1) // ipc_init()
  {
    fprintf(stderr, "ipc_init() failed\n");
    return -1;
  }

  // Start threads
  pthread_t sender, recver;
  pthread_create(&sender, NULL, (void *) start_sending, NULL);
  pthread_create(&recver, NULL, (void *) start_recving, NULL);

  pthread_join(sender, NULL);
  pthread_join(recver, NULL);

  // Close IPC
  ipc_close();

  // done
  return 0;
}

void start_sending()
{
  for(;;)
  {
    // Get command from user
    char cmd[MAX_MSG_LEN];
    printf("Enter command: ");
    fgets(cmd, MAX_MSG_LEN, stdin);

    // Check if exit
    if(strncmp(cmd, "exit", 4) == 0) // user wants to exit
    {
      break;
    }

    // Create placeholders for dest and msg
    char dest[3], msg[MAX_MSG_LEN];

    // Copy dest into dest placeholder
    strncpy(dest, cmd, 3);

    // Copy rest into msg
    int cmd_len = strlen(cmd);
    for(int x = 4; x < cmd_len; x++) msg[x-4] = cmd[x];

    // Add null termination characters
    msg[strlen(cmd)-4] = '\0';

    // Send message to process
    if(ipc_send(dest, msg, strlen(msg)) == -1) // ipc_send() failed
    {
      fprintf(stderr, "ipc_send() failed\n");
      return;
    }
  }
}

void start_recving()
{
  // Create placeholder for incoming message
  char msg[MAX_MSG_LEN];

  for(;;)
  {
    // Wait for incoming message
    if(ipc_recv("*", msg, MAX_MSG_LEN) == -1) // ipc_recv() failed
    {
      fprintf(stderr, "ipc_recv() failed\n");
      return;
    }

    // Print message on screen
    printf("Incoming msg: %s", msg);
  }
}
