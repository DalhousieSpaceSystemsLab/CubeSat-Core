/**
 * @file rf_module_settings.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Global settings header for the RF module
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_RF_INCLUDE_RF_MODULE_SETTINGS_H
#define CUBESAT_CORE_MODULES_RF_INCLUDE_RF_MODULE_SETTINGS_H

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
#define REQ_TAKE_PICTURE "TP"
#define REQ_ENCODE_FILE "EF"
#define REQ_DECODE_FILE "DF"
#define REQ_SHUTDOWN "SD"
#define REQ_REBOOT "RB"
#define REQ_REMOVE "RM"
#define REQ_MOVE "MV"

// Files
#define FILE_BASIC_TELEMETRY "telemetry-small.txt"
#define FILE_LARGE_TELEMETRY "telemetry-large.txt"
#define FILE_INCOMING "incoming.txt"
#define FILE_LS_INDEX "ls_index.txt"
#define MAX_FILENAME_LEN 64
#define FILE_NOT_FOUND "!!FNF!!"

#endif