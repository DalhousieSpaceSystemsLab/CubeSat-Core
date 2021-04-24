/*
 * conn_t.c
 *
 *   purpose:  provides standardized functions for 2 connection sockets
 *             to enable asynchronous two-way communication.
 *   author:   alex amellal
 *
 */

// Connection type
#include "conn_t.h"

// Returns initialized client_t
conn_t conn_t_new() {
  // Create placeholder for connection
  conn_t conn = {.rx = -1, .tx = -1};

  return conn;
}

// Returns connection status (0 = connected, -1 = closed)
int conn_t_stat(conn_t conn) {
  // Check if connections closed
  if (conn.rx == -1 || conn.tx == -1) return -1;

  // done
  return 0;
}

// Closes connection
int conn_t_close(conn_t* conn) {
  // Check if connections never initialized, otherwise close.
  if (conn->rx != -1) close(conn->rx);
  if (conn->tx != -1) close(conn->tx);

  // Reset connections
  conn->rx = -1;
  conn->tx = -1;

  // done
  return 0;
}