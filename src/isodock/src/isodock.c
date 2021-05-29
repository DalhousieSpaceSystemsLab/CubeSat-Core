/**
 * @file isodock.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Runs modules in isolation
 * @version 0.1
 * @date 2021-05-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

// Define dock settings
#define MODULE_STACK_SIZE 1024 * 512

// Import all the modules
#include "gps_module.h"
#include "mission_module.h"
#include "payload_module.h"

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("not enough args\n");
    return 0;
  }

  char* stack = (char*)malloc(MODULE_STACK_SIZE);
  if (stack == NULL) {
    printf("could not malloc heap\n");
    return 0;
  }

  int (*start_method)(void*);

  switch (argv[1][0]) {
    case 'g':
      start_method = gps.start;
      break;
    case 'm':
      start_method = mission.start;
      break;
    case 'p':
      start_method = payload.start;
      break;
    default:
      start_method = gps.start;
  };

  pid_t child_pid =
      clone(start_method, &stack[MODULE_STACK_SIZE], SIGCHLD, NULL);

  int status;
  waitpid(child_pid, &status, 0);

  // Check if process was terminated by a signal
  if (WIFSIGNALED(status)) {
    // DEBUG
    printf("process terminated by signal! sig = %d\n", WTERMSIG(status));
  }

  free(stack);

  return 0;
}