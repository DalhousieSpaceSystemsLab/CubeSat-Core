#include "twin.h"

int twin_server_start(void* data) { 
  printf("Starting twin!\n");
  for(int x = 0; x < 4; x++) {
    printf("running twin...\n");
    sleep(1);
  }
}

int twin_server_stop(void* data) {
  printf("Stopping twin!\n");
  fflush(stdout);
}

SubsystemModule twin_server = {
  .start = twin_server_start,
  .stop = twin_server_stop,
};