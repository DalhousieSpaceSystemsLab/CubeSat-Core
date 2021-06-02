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

    // Check command
    if (ipc_check_cmd(cmd, "%s %s", ipc.core.cmd.cmd.take_picture, "gps")) {
      // Get args
      int argc = ipc_get_n_args(cmd, strlen(cmd));
      char args[argc][MAX_ARG_LEN];
      OK(ipc_get_args(cmd, strlen(cmd), args, argc));

      // Check argc
      if (argc != 4) {
        moderr("Invalid number of arguments. SKIPPING\n");
        continue;
      }

      // Get coordinate floats
      float lattitude, longitude;
      lattitude = atof(args[2]);
      longitude = atof(args[3]);

      modprintf("About to ask mission to take picture at coordinates %f, %f\n",
                lattitude, longitude);

      // Forward comand to the mission module
      OK(ipc_send_cmd(ipc.core.msn.name, "%s %s %f %f", ipc.core.msn.cmd.qmsn,
                      "gps", lattitude, longitude));
    } else if (ipc_check_cmd(cmd, "%s %s", ipc.core.cmd.cmd.take_picture,
                             "time")) {
      // Get args
      int argc = ipc_get_n_args(cmd, strlen(cmd));
      char args[argc][MAX_ARG_LEN];
      OK(ipc_get_args(cmd, strlen(cmd), args, argc));

      // Check argc
      if (argc != 3) {
        moderr("Invalid number of arguments. SKIPPING\n");
        continue;
      }

      modprintf("About to ask mission to take picture at time %s\n", args[2]);
      // Forward comand to the mission module
      OK(ipc_send_cmd(ipc.core.msn.name, "%s %s %s", ipc.core.msn.cmd.qmsn,
                      "time", "2021-06-03"));
    }
  }
}

STOP_MODULE(command) {
  // Disconnect from the IPC
  OK(ipc_disconnect());
}

EXPORT_MODULE(command);