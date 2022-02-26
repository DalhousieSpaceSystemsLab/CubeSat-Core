/**
 * @file command_module_util.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Utility functions for the command module
 * @version 0.1
 * @date 2021-11-19
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_MODULES_COMMAND_MODULE_UTIL_H
#define CUBESAT_CORE_MODULES_COMMAND_MODULE_UTIL_H

// Project headers
#include "command_module_commands.h"
#include "ipc_settings.h"
#include "ipc_std.h"
#include "modutil.h"

// Standard C libraries
#include <inttypes.h>
#include <time.h>

/**
 * @brief Parses a command for the command module and returns a command ID.
 * See command_module_commands.h for the list of commands and their respective
 * IDs.
 *
 * @param cmd Message containing the command to parse.
 * @param cmd_len Length of the message to interpret in bytes.
 * @return Returns a command ID or 0 if no command identified.
 */
int cmd_parse(char* cmd, size_t cmd_len);

/**
 * @brief Parses and executes requirements for provided command.
 *
 * @param cmd String containing command to parse.
 * @param cmd_len Length of cmd.
 * @return 0 = OK, -1 = ERROR
 */
int cmd_handle(char* cmd, size_t cmd_len);

#endif