/**
 * filesystem.c
 * 
 * purpose: Stores, retrieves and organizes files on the 
 *          satellite's filesystem.
 * author:  alex amellal 
 * 
 */

#include "subsystem/servers/filesystem.h"

// Public server control methods 
void* filesystem_server_start();
void  filesystem_server_stop();

// Server container 
SubsystemModule filesystem_server = {
  .start  = filesystem_server_start,
  .stop   = filesystem_server_stop,
};

// Start filesystem server 
void* filesystem_server_start() {
  // Connect to the IPC
  if(ipc_connect("fio") != 0) {
    fprintf(stderr, "ipc_connect() failed : filesystem_server_start() failed\n");
    return NULL;
  }

  // 
  
  for(;;) {

  }
}