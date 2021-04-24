/**
 * json.h
 *
 * purpose: provide reusable JSON manipulation methods using JSMN library
 * author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_JSON_H
#define CUBESAT_CORE_INCLUDE_IPC_JSON_H

#ifdef __cplusplus
extern "C" {
#endif

// Project headers
#include "ipc_settings.h"

// Standard C libraries
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Custom JSON key-value pair
typedef struct JSON {
  char key[JSON_KEY_LEN];
  char val[JSON_VAL_LEN];
} json_t;

// Parse JSON string into key-value pairs.
// RETURN number of parsed key-value pairs.
int json_parse(char *json, size_t json_len, json_t *kv_pairs, size_t max_pairs);

// Stringify key-value pairs into JSON string
// RETURN length of stringified JSON.
int json_stringify(json_t *kv_pairs, size_t kv_pairs_len, char *json_str,
                   size_t max_json_str_len);

// Attenpts to parse string into JSON and reports if valid JSON or not
// RETURN true/false if JSON valid or not
bool json_test(char *json, size_t json_len);

// Attenpts to parse string into JSON and reports if valid JSON or not
// RETURN true/false if JSON valid or not
// NOTE: This function uses <n> key-value pair limit to test parsing.
bool json_ntest(char *json, size_t json_len, size_t n);

#ifdef __cplusplus
}
#endif

#endif  // End of CUBESAT_CORE_INCLUDE_IPC_JSON_H header guard