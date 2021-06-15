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
      if (argc != 6) {
        moderr("Invalid number of arguments. SKIPPING\n");
        continue;
      }

      // Get coordinate floats
      float gps_min[2], gps_max[2];
      gps_min[0] = atof(args[2]);
      gps_min[1] = atof(args[3]);
      gps_max[0] = atof(args[4]);
      gps_max[1] = atof(args[5]);

      modprintf(
          "About to ask mission to take picture at coordinates (min) %f, %f, "
          "(max) %f, %f\n",
          gps_min[0], gps_min[1], gps_max[0], gps_max[1]);

      // Forward command to the mission module
      OK(ipc_send_cmd(ipc.core.msn.name, "%s %s %s %f %f %f %f",
                      ipc.core.msn.cmd.qmsn, "gps",
                      ipc.core.cmd.cmd.take_picture, gps_min[0], gps_min[1],
                      gps_max[0], gps_max[1]));
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

      // Get time
      time_t t = atol(args[2]);

      // Forward to mission module
      OK(ipc_send_cmd(ipc.core.msn.name, "%s %s %s %ld", ipc.core.msn.cmd.qmsn,
                      "time", ipc.core.cmd.cmd.take_picture, t));
    }
  }
}

STOP_MODULE(command) {
  // Disconnect from the IPC
  OK(ipc_disconnect());
}

EXPORT_MODULE(command);