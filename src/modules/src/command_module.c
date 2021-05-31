/**
 * @file command_module.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Module dedicated to interpreting commands from mission control
 * @version 0.1
 * @date 2021-05-31
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "command_module.h"

START_MODULE(command) {
  // Connect to the IPC
  OK(ipc_connect(ipc.core.cmd.name));

  for (;;) {
    // Start listening for commands
    int bytes_recvd;
    char cmd[MAX_MSG_LEN];
    IF_TIMEOUT((bytes_recvd = ipc_recv("*", cmd, MAX_MSG_LEN, NO_TIMEOUT)),
               continue);

    // Parse command arguments
    int n_args;
    int max_n_args = 5;
    char args[max_n_args][MAX_ARG_LEN];
    OK((n_args = ipc_args(cmd, bytes_recvd, args, max_n_args)));

    // Print args received
    for (int x = 0; x < n_args; x++) {
      modprintf("arg%d = %s\n", x, args[x]);
    }
  }
}

STOP_MODULE(command) {
  // Disconnect from the IPC
  OK(ipc_disconnect());
}

EXPORT_MODULE(command);