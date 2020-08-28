/**
 * dock.c
 * 
 * purpose: act as a containerized executable platform for subsystem servers 
 * author: alex amellal 
 * 
 */

// Project headers
#include "subsystem/server_container_t.h"
#include "subsystem/servers/template.h"

// Standard C libraries
#include <stdlib.h>

int main() {
  // Initialize subsystem server container array
  subsystem_server_container_t servers[] = {
      template_server,

      SUBSYSTEM_SERVER_EOF,
  };

  // Calculate subsystem server array length
  int servers_len = subsystem_server_container_t_array_len(servers);

  // Initialize containers
  for (int x = 0; x < servers_len; x++) {
  }

  return 0;
}