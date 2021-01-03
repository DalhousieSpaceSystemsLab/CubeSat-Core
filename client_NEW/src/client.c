/*
 * client.c
 *
 *   purpose: Uses the client API to connect to the IPC.
 *   author: alex amellal
 *
 */

// Project headers
#include "client_api.h"

// Standard C libraries
#include <sys/select.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    // Check argc
    // if(argc != 2)
    if (argc != 3)
    {
        fprintf(stderr, "Invalid number of arguments\n Try: ./client <name> "
                        "<read/write>\n");
        return -1;
    }

    // Create placeholder for client name
    char *name = argv[1];
    char *rdwr = argv[2];

    // Check name length
    if (strlen(name) != 3)
    { // name is not correct length
        fprintf(stderr, "name must be 3 characters long\n");
        return -1;
    }

    // Connect to IPC system
    if (ipc_connect(name) == -1)
    { // ipc_connect() failed
        fprintf(stderr, "Failed to connect to the ipc\n");
        return -1;
    }

    // Check if client reading or writing
    if (strcmp(rdwr, "read") == 0)
    { // reading
        for (;;)
        {
            // Create placeholder for incoming message
            char msg[MAX_MSG_LEN];

            // Read data
            int bytes_read = -1;
            if ((bytes_read = ipc_recv("*", msg, MAX_MSG_LEN)) == -1)
            { // ipc_recv() failed
                fprintf(stderr, "ipc_recv() failed\n");
                return -1;
            }

            // Print data
            printf("message received: %s\n", msg);
        }
    }

    else if (strcmp(rdwr, "write") == 0)
    { // writing
        for (;;)
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
            if (strcmp(msg, "quit") == 0)
            { // user is asking to quit
                break;
            }

            // Send message to other client
            if (ipc_send(dest, msg, strlen(msg)) == -1)
            { // ipc_send() failed
                fprintf(stderr, "ipc_send() failed\n");
                return -1;
            }
        }
    }

    else if (strcmp(rdwr, "async") == 0)
    { // async communication
        // Create placeholders for messages
        char msg_recv[MAX_MSG_LEN];
        char msg_send[MAX_MSG_LEN + 2];

        for (;;)
        {
            // Prompt user for read
            char read[3];
            printf("read message? (y/n) : ");
            fgets(read, 3, stdin);

            // Prompt user for write
            char write[3];
            printf("write message? (y/n) : ");
            fgets(write, 3, stdin);

            // Queue read if desired
            if (read[0] == 'y')
            {
                if (ipc_qrecv("*", msg_recv, MAX_MSG_LEN) == -1)
                { // ipc_qrecv() failed
                    fprintf(stderr, "ipc_qrecv() failed\n");
                    return -1;
                }
            }

            // Queue send if desired
            if (write[0] == 'y')
            {
                // Placeholder for destination name
                char dest[NAME_LEN + 2];

                // Prompt user for input
                printf("Enter destination name: ");
                fgets(dest, NAME_LEN + 2, stdin);
                printf("Enter message: ");
                fgets(msg_send, MAX_MSG_LEN + 2, stdin);

                // Queue message send
                if (ipc_qsend(dest, msg_send, strlen(msg_send)) == -1)
                { // ipc_qsend() failed
                    fprintf(stderr, "ipc_qsend() failed\n");
                    return -1;
                }
            }

            else if (read[0] == 'n' && write[0] == 'n')
            { // quit
                ipc_disconnect();

                // quit
                return 0;
            }

            // Refresh IPC queue
            if (ipc_refresh() == -1)
            { // ipc_refresh() failed
                fprintf(stderr, "ipc_refresh() failed\n");
                return -1;
            }

            // Check if message received
            if (msg_recv[0] != 0)
            {
                printf("message received: %s\n", msg_recv);
            }
        }
    }

    else
    { // bad keyword
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