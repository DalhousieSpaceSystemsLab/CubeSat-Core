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