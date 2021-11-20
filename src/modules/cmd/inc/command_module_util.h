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

int parse_cmd(char* cmd, size_t cmd_len);

#endif