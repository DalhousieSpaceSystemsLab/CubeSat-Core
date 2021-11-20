/**
 * @file command_module_commands.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Collection of commands for the LORIS satellite
 * @version 0.1
 * @date 2021-11-19
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_MODULES_COMMAND_MODULE_COMMANDS_H
#define CUBESAT_CORE_MODULES_COMMAND_MODULE_COMMANDS_H

#define CMD_TAKE_PICTURE_GPS cmd, "%s %s", ipc.core.cmd.cmd.take_picture, "gps"
#define CMD_TAKE_PICTURE_TIME \
  cmd, "%s %s", ipc.core.cmd.cmd.take_picture, "time"

enum {
  CMD_TAKE_PICTURE_GPS_ID = 100,
  CMD_TAKE_PICTURE_TIME_ID,
};

#endif