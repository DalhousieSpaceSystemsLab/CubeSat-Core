/*
* msnclient.c
*
*   purpose: Uses the client API to connect to the IPC.
*   author: Yann Guerin
*
*/

// Project headers
#include "ipc/client_api.h"

// Standard C Libraries
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

void start_sending();
void start_recving();
const char *get_msn();
int is_current_min(char*);

//Recommendations:
  //Add coordinate checks...
  //Run functions from queue array, instead of file itself
  //Create corruption handling

    //add pointer to sorting function to return the array
    //use existing code to create a function that works from array to extract info
    //modify sorting function to return the index or value of the mission that starts the soonest

// *** To add to get_msn:
  //Working with arrays not lists,
  //Working with coordinates,
  //Integrating with ipc/api,
  //Increase robustness

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
    // Get mission from msnqueue
    const char *message[MAX_MSG_LEN];
    message = get_msn();

    // Check if exit
    if(strncmp(message, "exit", 4) == 0) // user wants to exit
    {
      break;
    }

    // Create placeholders for dest and msg
    char dest[3], msg[MAX_MSG_LEN];

    // Copy dest into dest placeholder
    strncpy(dest, message, 3);

    // Copy rest into msg
    int message_len = strlen(message);
    for(int x = 4; x < cmd_len; x++) msg[x-4] = message[x];

    // Add null termination characters
    msg[strlen(message)-4] = '\0';

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

const char *get_msn()
{
  //initializing message variable to store the target and mission to be sent
  char* message = malloc(256);
  //initralizing filename to access
  static const char filename[] = "msnqueue.txt";

  //open file
  FILE *file = fopen(filename, "r");
  int count = 0;
  if ( file != NULL )
  {
      //loop through each line of msnqueue.txt file
      for(int lineNumber=0; lineNumber<6; lineNumber++)
      {
        //inializing length of line placeholder
        int line_len = 256;
        //initializing placeholder for line data
        char line[line_len];
        //initializing placeholder for mission time
        char msnTime[25];

        //get line in the file
        while (fgets(line, line_len, file) != NULL)
        {
            //** could maybe be removed
            if (count == lineNumber) //if the current line is the specified one
            {
                //parse the first 24 characters in the line (the date and time) and store in msnTime
                for(int i = 0; i < 24; i++)
                {
                  msnTime[i] = line[i];
                }
                msnTime[24] = '\0';
                printf("Mission time: %s\n", msnTime);

                if(is_current_min(msnTime) == 0)
                {
                  printf("Mission minute is current minute\n");

                  int k = 0;
                  //parse the remainder of the line (the mission) and store in mission
                  for(int i = 25; i < line_len; i++)
                  {
                    message[k] = line[i];
                    k++;
                  }
                  printf("The message: %s\n", message);
                  break;

                } else {
                  printf("Not time yet!\n\n");
                  continue;
                }
            } else {
                count++;
                continue;
            }
        }
      }
      fclose(file);
  } else {
    printf("Error opening file");
    message = "exit";
  }
  return message;
}

int is_current_min(char* msnTime)
{
  //initialize a time variable and double variable
  time_t start_t;
  double diff_t;
  //access the current time
  time(&start_t);
  //store current time in tm struct
  struct tm *currentTime = localtime(&start_t);

  //create a struct placeholder for the data and time
  struct tm msnTM;
  //turn the string containing date and time into a time tm struct
  strptime(msnTime, "%a %b %d %H:%M:%S %Y", &msnTM);

  //if(currentTime->tm_min == msnTM.tm_min)
  if(55 == msnTM.tm_min)
  {
    return 0;
  } else {
    return -1;
  }
}
