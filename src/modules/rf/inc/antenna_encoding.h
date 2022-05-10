/**
 * @file antenna_encoding.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Provide reed-solomon encoding support for the antenna
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_RF_INCLUDE_ANTENNA_ENCODING_H
#define CUBESAT_CORE_MODULES_RF_INCLUDE_ANTENNA_ENCODING_H

// Project headers
#include "modutil.h"
#include "rf_module_settings.h"

// External libraries
#include "ecc.h"

// Standard C libraries
#include <stdio.h>
#include <string.h>

// Settings
#define BLOCK_SIZE 15
#define ENCODE_SIZE BLOCK_SIZE + NPAR

/**
 * @brief Initialize the encoding lib
 *
 * @return int
 */
int antenna_encode_init();

/**
 * @brief Encode file and save as encoded-<name of file>
 *
 * @param path Path to file in question
 * @return int 0 = OK, -1 = ERR
 */
int antenna_encode_file(const char *path);

/**
 * @brief Decode file and save as decoded-<name of file>. Attempts to replace
 * encoded prefix as well
 *
 * @param path Path to file in question
 * @return int 0 = OK, -1 = ERR
 */
int antenna_decode_file(const char *path);

#endif