/**
 * @file imu_module.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief IMU support for LORIS
 * @version 0.1
 * @date 2022-05-29
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_IMU_INCLUDE_IMU_MODULE_H
#define CUBESAT_CORE_MODULES_IMU_INCLUDE_IMU_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C libs
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

// Settings
#define IMU_DEV_PATH "/dev/colibri-uartc"
#define IMU_DATA_FILE "imu.txt"
#define RAVEN_UART_SPEED B115200
#define MAX_ENTRY_LEN 128
#define MAX_DATESTAMP_SIZE 32
#define MAX_FILENAME_SIZE MAX_DATESTAMP_SIZE + 32
#define LOG_DIRECTORY "imu_logs"
#define FILE_INTERVAL 90 * 60  // in seconds
#define LOG_INTERVAL 1000000   // in microseconds
#define ADCS_DEV_PATH "/sys/class/gpio/gpio114/value"
#define GPIO_ENABLE "1"

NEW_DOCK_MODULE(imu);

#endif