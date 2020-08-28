/**
* container_t.c
*
*   purpose: provide a modular placeholder for subsystem servers (container)
*   author: alex amellal
*
*/

#include "subsystem/server_container_t.h"

size_t subsystem_server_container_t_array_len(subsystem_server_container_t* array) {
  size_t len = 0;
  for (size_t x = 0; x < MAX_NUM_SUBSYSTEM_SERVERS; x++) {
    if (array[x].start == SUBSYSTEM_SERVER_EOF.start && array[x].stop == SUBSYSTEM_SERVER_EOF.stop) {
      break;
    } else {
      len++;
    }
  }

  // done
  return len;
}