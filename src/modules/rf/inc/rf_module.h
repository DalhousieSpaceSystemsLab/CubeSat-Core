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
#include "subsysmod.h"

// Standard C libraries
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

// Settings
#define PATH_TO_ANTENNA_DEV "/dev/colibri-uartb"

// Requests
#define REQ_BASIC_TELEMETRY "A1"
#define REQ_LARGE_TELEMETRY "B2"
#define REQ_DELET_TELEMETRY "C3"
#define REQ_REBOOT_OBC "D4"
#define REQ_RESET_COMMS "E5"
#define REQ_ENABLE_RAVEN "F6"
#define REQ_FWD_COMMAND "CC"
#define REQ_LISTEN_FILE "FF"
#define REQ_GET_FILE "FR"
#define REQ_GET_LS "LS"

// Files
#define FILE_BASIC_TELEMETRY "telemetry.txt"
#define FILE_INCOMING "incoming.txt"
#define FILE_LS_INDEX "ls_index.txt"
#define MAX_FILENAME_LEN 64
#define FILE_NOT_FOUND "!!FNF!!"

NEW_DOCK_MODULE(rf);

#endif