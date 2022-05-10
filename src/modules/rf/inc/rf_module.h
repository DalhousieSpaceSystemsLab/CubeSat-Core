/**
 * @file rf_module.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Radio module for the satellite
 * @version 0.1
 * @date 2021-06-02
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

/**
 * NOTES:
 *
 * - At the moment, READ_MODE_UPTO probably only works in increments of 64 bytes
 * (because that is the write buffer size).
 * - Functions like listen_file() probably depend on this trait for filenames to
 * be read before the incoming file is listen for in READ_MODE_UPTO.
 * - IF ever the size of the write buffer is changed or if filenames surpass the
 * buffer size, there will likely be an issue reading them.
 */

/**
 * TODO:
 *
 * - Implement timeout for every autonomous function to prevent locking up. Must
 * be possible at every step.
 */

#ifndef CUBESAT_CORE_MODULES_RF_MODULE_H
#define CUBESAT_CORE_MODULES_RF_MODULE_H

#include "antenna.h"
#include "rf_module_settings.h"
#include "subsysmod.h"

// Standard C libraries
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

NEW_DOCK_MODULE(rf);

#endif