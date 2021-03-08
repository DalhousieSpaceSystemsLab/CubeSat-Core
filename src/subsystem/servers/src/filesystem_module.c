/**
 * filesystem.c
 * 
 * purpose: Stores, retrieves and organizes files on the 
 *          satellite's filesystem.
 * author:  alex amellal 
 * 
 */

#include "filesystem_module.h"
#include "client_api.h"

// Public server control methods 
int filesystem_server_start(void*);
int filesystem_server_stop(void*);

// Callback methods 
static void cb_general(char *msg, size_t msg_len, void * data);

// Module functions 

// Server container 
SubsystemModule filesystem_module = {
  .start  = filesystem_server_start,
  .stop   = filesystem_server_stop,
};

// Start filesystem server 
int filesystem_server_start(void* data) {
  // Connect to the IPC
  if(ipc_connect(ipc.core.fls.name) != 0) {
    fprintf(stderr, "[fls] ipc_connect() failed : filesystem_server_start() failed\n");
    return 0;
  }

  // Create listener for general requests 
  if(ipc_qrecv("*", cb_general, NULL, IPC_QRECV_MSG) < 0) {
    fprintf(stderr, "[fls] ipc_qrecv(\"*\") failed\n");
    return 0;
  }
  
  for(;;) {
    if(ipc_refresh() < 0) {
      fprintf(stderr, "[fls] ipc_refresh() failed");
      return 0;
    }
  }
}

int filesystem_server_stop(void* data) {
  // Disconnect from the IPC 
  ipc_disconnect();
}

static void cb_general(char *msg, size_t msg_len, void * data) {
  
}