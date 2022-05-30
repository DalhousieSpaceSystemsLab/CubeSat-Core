/**
 * @file raven_module.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Raven telemetry support for LORIS
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_RAV_INC_RAVEN_MODULE_H
#define CUBESAT_CORE_MODULES_RAV_INC_RAVEN_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C libs
#include <time.h>

// Settings
#define GPIO_PATH "/sys/class/gpio/gpio122"
#define FILE_INTERVAL 90 * 60  // in seconds
#define LOG_DIRECTORY "raven_logs"
#define MAX_DATESTAMP_SIZE 32
#define MAX_FILENAME_SIZE MAX_DATESTAMP_SIZE + 32
#define LOG_INTERVAL 200000  // in microseconds

NEW_DOCK_MODULE(rav);

#endif