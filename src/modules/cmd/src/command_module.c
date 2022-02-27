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

    // Parse command
    // int cmd_id = cmd_parse(cmd, strlen(cmd));
    OK(cmd_handle(cmd, strlen(cmd)));
  }
}

STOP_MODULE(command) {
  // Disconnect from the IPC
  OK(ipc_disconnect());
}

EXPORT_MODULE(command);