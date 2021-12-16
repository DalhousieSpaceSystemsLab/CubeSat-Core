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
#include "client_api.h"
#include "command_module_commands.h"
#include "ipc_std.h"

// Standard C libraries
#include <stddef.h>

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

#endif