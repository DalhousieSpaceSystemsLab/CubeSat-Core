/**
 * @file antenna_encoding.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief File error correction support for LORIS
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_RF_INCLUDE_ANTENNA_ENCODING_H
#define CUBESAT_CORE_MODULES_RF_INCLUDE_ANTENNA_ENCODING_H

// Project headers
#include "modutil.h"
#include "rf_module_settings.h"

// Standard C libs
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Encode file and save encoded result.
 *
 * @param file_in Path to file to encode
 * @param file_out Output path for encoded file
 * @return 0 = OK, -1 = ERR
 */
int antenna_encode_file(const char* file_in, const char* file_out);

/**
 * @brief Decode file and save data.
 *
 * @param file_in Path to file to decode
 * @param file_out Output path for decoded result
 * @return 0 = OK, -1 = ERR
 */
int antenna_decode_file(const char* file_in, const char* file_out);

#endif