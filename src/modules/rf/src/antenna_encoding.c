/**
 * @file antenna_encoding.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief File error correction support for LORIS
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "antenna_encoding.h"

/**
 * @brief Encode file and save encoded result.
 *
 * @param file_in Path to file to encode
 * @param file_out Output path for encoded file
 * @return 0 = OK, -1 = ERR
 */
int antenna_encode_file(const char* file_in, const char* file_out) {
  // Prepare command
  char command[2 * MAX_FILENAME_LEN + 24];
  sprintf(command, "clrfec encode %s %s", file_in, file_out);

  // Encode
  system(command);

  // done
  return 0;
}

/**
 * @brief Decode file and save data.
 *
 * @param file_in Path to file to decode
 * @param file_out Output path for decoded result
 * @return 0 = OK, -1 = ERR
 */
int antenna_decode_file(const char* file_in, const char* file_out) {
  // Prepare command
  char command[2 * MAX_FILENAME_LEN + 24];
  sprintf(command, "clrfec decode %s %s", file_in, file_out);

  // Encode
  system(command);

  // done
  return 0;
}