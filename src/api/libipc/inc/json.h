/**
 * json.h
 * 
 * purpose: provide reusable JSON manipulation methods using JSMN library 
 * author:  alex amellal
 * 
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_JSON_H
#define CUBESAT_CORE_INCLUDE_IPC_JSON_H

// Project headers
#include "settings.h"

// Standard C libraries 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Custom JSON key-value pair 
typedef struct JSON_KV {
  char key[JSON_KEY_LEN];
  char val[JSON_VAL_LEN];
} json_kv_t;

int json_parse(char *json, size_t json_len, json_kv_t *kv_pairs, size_t max_pairs);
int json_stringify(json_kv_t *kv_pairs, size_t kv_pairs_len, char *json_str, size_t max_json_str_len);

#endif // End of CUBESAT_CORE_INCLUDE_IPC_JSON_H header guard