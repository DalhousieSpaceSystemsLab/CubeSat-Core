/**
 * dock.c
 * 
 * purpose: act as a containerized executable platform for subsystem servers 
 * author: alex amellal 
 * 
 */

#include "subsystem/dock.h"

// Private methods 
static int stop_server(ServerContainer* server);
static void* monitor_server(void* args);

// Start all server containers
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
  
  // Initialize containers
  for (int x = 0; x < servers_len; x++) {
    // Initialize server TIDs to blank value 
    servers[x].tid = -1;
    servers[x].mtid = -1;

    // Create start thread 
    if((errno = pthread_create(&servers[x].tid, NULL, servers[x].start, NULL)) != 0) {
      perror("pthread_create() failed");
      return -1;
    }

    // Start monitoring server 
    if((errno = pthread_create(&servers[x].mtid, NULL, monitor_server, (void *) &servers[x])) != 0) {
      perror("pthread_create() failed");
      return -1;
    }
  }

  return 0;
}

// Stop all server containers
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

  // Check if tid is initialized 
  if(server->tid != -1) {
    // Cancel start thread 
    pthread_cancel(server->tid);

    // Reset tid value for server 
    server->tid = -1;

    // Run stop function 
    server->stop();
  }

  // Check if mtid is initialized 
  if(server->mtid != -1) {
    // Cancel monitor thread 
    pthread_cancel(server->mtid);

    // Reset mtid value 
    server->mtid = -1;
  }
  
  // done
  return 0;
}

/**
 * Thread which continuously monitors for successful or erroneous exit 
 * of designated server's 'start()' method 
 */
static void* monitor_server(void* args) {
  // Convert void pointer args to ServerContainer 
  ServerContainer* server = (ServerContainer *) args;

  // Check if tid is initialized 
  if(server->tid == -1) {
    fprintf(stderr, "server container tid is not initialized. NOT MONITORING\n");
    return NULL;
  }

  for(;;) {
    // Wait on thread to join 
    pthread_join(server->tid, NULL);

    // Reset server tid 
    server->tid = -1;

    // Restart server 
    if((errno = pthread_create(&server->tid, NULL, server->start, NULL)) != 0) {
      perror("pthread_create() failed");
      return NULL;
    }
  }
}
