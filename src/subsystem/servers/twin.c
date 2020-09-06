#include "subsystem/servers/twin.h"

void * twin_server_start() { 
  printf("Starting twin!\n");
  for(;;) {
    printf("waiting...\n");
    sleep(1);
  }
}

void twin_server_stop() {
  printf("Stopping twin!\n");
  fflush(stdout);
}

ServerContainer twin_server = {
  .start = twin_server_start,
  .stop = twin_server_stop,
};