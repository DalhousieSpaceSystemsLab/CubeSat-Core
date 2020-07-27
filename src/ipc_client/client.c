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

int main(int argc, char * argv[])
{
  // Check argc
  // if(argc != 2)
  if(argc != 3)
  {
    fprintf(stderr, "Invalid number of arguments\n Try: ./client <name> <read/write>\n");
    return -1;
  }

  // Create placeholder for client name
  char * name = argv[1];
  char * rdwr = argv[2];

  // Check name length
  if(strlen(name) != 3) // name is not correct length
  {
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Connect to IPC system
  if(ipc_connect(name) == -1) // ipc_connect() failed
  {
    fprintf(stderr, "Failed to connect to the ipc\n");
    return -1;  
  }

  // Check if client reading or writing 
  if(strcmp(rdwr, "read") == 0) // reading
  {
    for(;;)
    {
      // Create placeholder for incoming message 
      char msg[MAX_MSG_LEN];

      // Read data 
      int bytes_read = -1;
      if((bytes_read = ipc_recv("*", msg, MAX_MSG_LEN)) == -1) // ipc_recv() failed 
      {
        fprintf(stderr, "ipc_recv() failed\n");
        return -1;
      }

      // Print data 
      printf("message received: %s\n", msg);
    }
  } 
  
  else if(strcmp(rdwr, "write") == 0) // writing 
  {
    for(;;)
    {
      // Create placeholders for message to send 
      char dest[NAME_LEN + 2];
      char msg[MAX_MSG_LEN + 2];

      // Ask user for message and destination 
      printf("Enter destination's name: ");
      fgets(dest, NAME_LEN + 2, stdin);

      // Ask user for message 
      printf("Enter message: ");
      fgets(msg, MAX_MSG_LEN + 2, stdin);

      // Check if message 'quit'
      if(strcmp(msg, "quit") == 0) // user is asking to quit 
      {
        break;
      }

      // Send message to other client 
      if(ipc_send(dest, msg, strlen(msg)) == -1) // ipc_send() failed 
      {
        fprintf(stderr, "ipc_send() failed\n");
        return -1;
      }
    }
  }

  else if(strcmp(rdwr, "async") == 0) // async communication
  {
    // Prompt user 
    printf("Enter message: ");
    fflush(stdout);
    
    for(;;)
    {
      // Create placeholders for select function 
      fd_set readstdin;
      struct timeval tv;

      // Reset select placeholders
      FD_ZERO(&readstdin);
      FD_SET(fileno(stdin), &readstdin);

      // Set user read timeout 
      tv.tv_sec = 1;
      tv.tv_usec = 0;

      // Check for input 
      int readable = select(fileno(stdin)+1, &readstdin, NULL, NULL, &tv);

      // Check if user placed an input 
      if(readable == -1) // select() failed 
      {
        fprintf(stderr, "select() failed\n");
        return -1;
      }

      else if(readable == 0) // timeout 
      {
        // printf("timeout!\n");
      }

      else // input detected 
      {
        // Create placeholder for user's command 
        char cmd[MAX_MSG_LEN + 2];

        // Read incoming data 
        fgets(cmd, MAX_MSG_LEN + 2, stdin);

        // Create placeholders for destination + msg 
        char dest[NAME_LEN];
        char msg[MAX_MSG_LEN];

        // Parse command into destination + msg 
        strncpy(dest, cmd, 3);
        for(int x = (NAME_LEN+1); x < MAX_MSG_LEN; x++) 
        {
          // Check if newline character 
          if(cmd[x] == '\n') // end of command 
          {
            // Null terminate string 
            msg[x-(NAME_LEN+1)] = '\0';
            
            // done
            break;
          }

          // Copy character into msg 
          msg[x-(NAME_LEN+1)] = cmd[x];
        }

        // Print destination and message 
        printf("dest: ");
        for(int x = 0; x < NAME_LEN; x++) printf("%c", dest[x]);
        printf("\n");
        printf("msg: %s\n", msg);

        // Prompt user again 
        printf("Enter message: ");
        fflush(stdout);
      }
    }
  }

  else // bad keyword 
  {
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