/**
 * @file command_module_util.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Utility functions for the command module
 * @version 0.1
 * @date 2021-11-19
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "command_module_util.h"

int cmd_parse(char* cmd, size_t cmd_len) {
  // Get args
  int argc = ipc_get_n_args(cmd, cmd_len);
  char argv[argc][MAX_ARG_LEN];
  OK(ipc_get_args(cmd, cmd_len, argv, argc));

  // Check commands
  if (ipc_check_cmd(CMD_TAKE_PICTURE_GPS)) {
    return CMD_TAKE_PICTURE_GPS_ID;
  } else if (ipc_check_cmd(CMD_TAKE_PICTURE_TIME)) {
    return CMD_TAKE_PICTURE_TIME_ID;
  } else if (ipc_check_cmd(CMD_TAKE_PICTURE)) {
    return CMD_TAKE_PICTURE_ID;
  }

  // done, no command identified
  return 0;
}

/**
 * @brief Parses and executes requirements for provided command.
 *
 * @param cmd String containing command to parse.
 * @param cmd_len Length of cmd.
 * @return 0 = OK, -1 = ERROR
 */
int cmd_handle(char* cmd, size_t cmd_len) {
  // Parse command
  int cmd_id = cmd_parse(cmd, cmd_len);

  // Check if GPS condition provided
  if (cmd_id == CMD_TAKE_PICTURE_GPS_ID) {
    // Get args
    int argc = ipc_get_n_args(cmd, strlen(cmd));
    char args[argc][MAX_ARG_LEN];
    OK(ipc_get_args(cmd, strlen(cmd), args, argc));

    // Check argc
    if (argc != 6) {
      moderr("Invalid number of arguments. SKIPPING\n");
      return -1;
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
                    ipc.core.msn.cmd.qmsn, "gps", ipc.pay.cmd.take_pic,
                    gps_min[0], gps_min[1], gps_max[0], gps_max[1]));

    // Check if time condition provided
  } else if (cmd_id == CMD_TAKE_PICTURE_TIME_ID) {
    // Get args
    int argc = ipc_get_n_args(cmd, strlen(cmd));
    char args[argc][MAX_ARG_LEN];
    OK(ipc_get_args(cmd, strlen(cmd), args, argc));

    // Check argc
    if (argc != 3) {
      moderr("Invalid number of arguments. SKIPPING\n");
      return -1;
    }

    // Get time
    time_t t = atol(args[2]);

    modprintf("About to ask mission to take picture at time %ld\n", t);

    // Forward to mission module
    OK(ipc_send_cmd(ipc.core.msn.name, "%s %s %s %ld", ipc.core.msn.cmd.qmsn,
                    "time", ipc.pay.cmd.take_pic, t));

    // Check for commands for other subsystems
  } else if (cmd_id == CMD_TAKE_PICTURE) {
    // Send message to payload
    OK(ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic));
  }
}