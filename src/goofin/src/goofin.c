/**
 * @file goofin.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Coding sandbox for this repo; just for goofin & testing
 * @version 0.1
 * @date 2021-05-28
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include <stdio.h>
#include "subsysmod.h"

int main() {
  const char* raw_msg = "0x13248972 gps -12.234876 3.1415926535";
  char args[5][MAX_ARG_LEN];
  int n_args;
  OK((n_args = ipc_args(raw_msg, strlen(raw_msg), args, 5)));

  printf("n_args = %d\n", n_args);

  for (int x = 0; x < n_args; x++) {
    printf("[arg%d] %s\n", x, args[x]);
  }

  return 0;
}