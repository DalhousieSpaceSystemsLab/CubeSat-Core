/*
* bugs.c
*
*   purpose: collection of bugs and resolutions to encourage collaboration and progress
*
*/

/*
*   bug 1
*
*   description:  basically when a client tries to read and write from the IPC at the same time, it locks until it is done reading or writing.
*   solution:     created new conn_t to hold 2 sockets per client. one for rx and another for tx.
*
*   affected files:
*     - ipc.c
*     - client_t.h
*     - conn_t.h
*     - client_handler.c/.h
*     - (everything)
*/

typedef struct Conn
{
  int rx; // receiving connection socket
  int tx; // transmitting connection socket
} conn_t;

typedef struct Client
{
  conn_t  conn;     // connection socket placeholders
  char    name[3];  // client name placeholder
} client_t;

// Initialize client placeholders
for(int x = 0; x < MAX_NUM_CLI; x++)
{
  clients[x].conn.rx = -1;
  clients[x].conn.tx = -1;
}
