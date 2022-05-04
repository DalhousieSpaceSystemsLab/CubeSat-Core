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