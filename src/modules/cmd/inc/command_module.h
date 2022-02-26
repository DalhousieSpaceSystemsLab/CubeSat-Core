/**
 * @file command_module.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Module dedicated to interpreting commands from mission control
 * @version 0.1
 * @date 2021-05-31
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_MODULES_COMMAND_MODULE_H
#define CUBESAT_CORE_MODULES_COMMAND_MODULE_H

// Subsystem module header
#include "subsysmod.h"

// Module headers
#include "command_module_util.h"

NEW_DOCK_MODULE(command);

#endif