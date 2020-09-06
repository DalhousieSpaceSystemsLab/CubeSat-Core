/**
 * dock.c
 * 
 * purpose: act as a containerized executable platform for subsystem servers 
 * author: alex amellal 
 * 
 */

#include "subsystem/dock.h"

// Private methods 
static int stop_server(ServerContainer* server_tid);

int dock_start(ServerContainer* servers, size_t servers_len) {
  // Ensure servers pointer is not null 
  if(servers == NULL) {
    fprintf(stderr, "servers pointer cannot be null : ");
    return -1;
  }

  // Ensure length is a non-zero positive value 
  if(servers_len <= 0) {
    fprintf(stderr, "servers array length must be a non-zero positive value : ");
    return -1;
  }
  
  // Initialize server TID to blank value 
  for(int x = 0; x < servers_len; x++) {
    servers[x].tid = -1;
  }

  // Initialize containers
  for (int x = 0; x < servers_len; x++) {
    // Create start thread 
    if((errno = pthread_create(&servers[x].tid, NULL, servers[x].start, NULL)) != 0) {
      perror("pthread_create() failed");
      return -1;
    }
  }

  return 0;
}

int dock_stop(ServerContainer* servers, size_t servers_len) {
  // Shutdown all servers 
  for(int x = 0; x < servers_len; x++) {
    if(stop_server(&servers[x]) != 0) {
      fprintf(stderr, "stop_server() failed : ");
      return -1;
    }
  }

  // done
  return 0;
}

// Stop execution of server and run stop function associated to it 
static int stop_server(ServerContainer* server) {
  // Check if server pointer non null 
  if(server == NULL) {
    fprintf(stderr, "cannot use null pointer to server : ");
    return -1;
  }
  
  // Cancel start thread 
  pthread_cancel(server->tid);

  // Reset tid value for server 
  server->tid = -1;

  // Run stop function 
  server->stop();

  // done
  return 0;
}