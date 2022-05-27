/**
 * @file antenna_encoding.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Provide reed-solomon encoding support to the antenna
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "antenna_encoding_old.h"

/**
 * @brief Initialize the encoding lib
 *
 * @return int
 */
int antenna_encode_init() {
  initialize_ecc();
  return 0;
}

/**
 * @brief Encode file and save as encoded-<name of file>
 *
 * @param path Path to file in question
 * @return int 0 = OK, -1 = ERR
 */
int antenna_encode_file(const char *path) {
  int status = 0;

  // Try to open file
  FILE *fp = fopen(path, "r");
  if (!fp) {
    moderr("Failed to open file (%s) to encode\n", path);
    return -1;
  }

  // Generate encoded filename
  char newfilename[MAX_FILENAME_LEN];
  sprintf(newfilename, "%s.encoded", path);

  // Open new file to encode to
  FILE *newfp = fopen(newfilename, "w");
  if (!newfp) {
    moderr("Failed to open new file (%s) for encoding\n", newfilename);
    status = -1;
    goto cleanup;
  }

  // Parse file
  char nextblock[BLOCK_SIZE];
  char nextencode[ENCODE_SIZE];
  int done = 0;
  while (!done) {
    // Get block
    if (fread(nextblock, sizeof(char), BLOCK_SIZE, fp) < BLOCK_SIZE) {
      done = 1;
    }

    // Encode block
    encode_data(nextblock, BLOCK_SIZE, nextencode);

    // Write encoded block to new file
    if (fwrite(nextencode, sizeof(char), ENCODE_SIZE, newfp) < ENCODE_SIZE) {
      moderr("Failed to write to new file\n");
      status = -1;
      fclose(newfp);
      goto cleanup;
    }
  }

  // Close new file
  fclose(newfp);

cleanup:
  // Close file
  fclose(fp);

  // done
  return status;
}

/**
 * @brief Decode file and save as decoded-<name of file>. Attempts to replace
 * encoded prefix as well
 *
 * @param path Path to file in question
 * @return int 0 = OK, -1 = ERR
 */
int antenna_decode_file(const char *path) {
  int status = 0;

  // Try to open file
  FILE *fp = fopen(path, "r");
  if (!fp) {
    moderr("Failed to open file (%s) to encode\n", path);
    return -1;
  }

  // Generate encoded filename
  char newfilename[MAX_FILENAME_LEN];
  sprintf(newfilename, "%s.decoded", path);

  // Open new file to encode to
  FILE *newfp = fopen(newfilename, "w");
  if (!newfp) {
    moderr("Failed to open new file (%s) for encoding\n", newfilename);
    status = -1;
    goto cleanup;
  }

  // Parse file
  char nextblock[BLOCK_SIZE];
  char nextencode[ENCODE_SIZE];
  int done = 0;
  while (!done) {
    // Get block
    if (fread(nextencode, sizeof(char), ENCODE_SIZE, fp) < ENCODE_SIZE) {
      done = 1;
    }

    // Decode block
    decode_data(nextencode, ENCODE_SIZE);
    int err_corr = 0;
    if (check_syndrome()) {
      err_corr = correct_errors_erasures(nextencode, ENCODE_SIZE, 0, NULL);
    }
    memcpy(nextblock, nextencode, BLOCK_SIZE);

    // Write encoded block to new file
    if (fwrite(nextblock, sizeof(char), BLOCK_SIZE, newfp) < BLOCK_SIZE) {
      moderr("Failed to write to new file\n");
      status = -1;
      fclose(newfp);
      goto cleanup;
    }
  }

  // Close new file
  fclose(newfp);

cleanup:
  // Close file
  fclose(fp);

  // done
  return status;
}