/**
 * json.c
 * 
 * purpose: provide reusable JSON manipulation methods using JSMN library 
 * author:  alex amellal
 * 
 */

#include "json.h"
#include "jsmn.h"

// Parse JSON string into key-value pairs
int json_parse(char *json, size_t json_len, json_t *kv_pairs, size_t max_pairs) {
  // Initialize json parser 
  jsmn_parser parser;
  jsmn_init(&parser);

  // Allocate jsmn tokens 
  jsmntok_t tokens[max_pairs + 1];

  // Parse JSON
  int tokens_parsed;
  if((tokens_parsed = jsmn_parse(&parser, json, json_len, tokens, max_pairs+1)) < 0) { 
   switch(tokens_parsed) {
    case JSMN_ERROR_INVAL:
      fprintf(stderr, "bad token, JSON string is corrupted :");
      break;
    case JSMN_ERROR_NOMEM:
      fprintf(stderr, "not enough tokens, JSON string is too large :");
      break;
    case JSMN_ERROR_PART:
      fprintf(stderr, "JSON string is too short, expecting more JSON data :");
      break;
    default:
      fprintf(stderr, "unknown error :");
   }

    fprintf(stderr, "jsmn_parse() failed : ");
    return -1;
  }

  // Convert jsmn tokens into string key-value pairs 
  bool key_stored = false;
  int nth_pair = 0;
  for(int x = 0; x < tokens_parsed && nth_pair < max_pairs; x++) {
    // Token contains string. Alternate between storing key and value.
    if(tokens[x].type == JSMN_STRING) {
      // Make calculations to copy string 
      int start_index = tokens[x].start;
      int str_len     = tokens[x].end - tokens[x].start;

      // Check if key needed for next pair 
      if(!key_stored) {
        // Copy token string into key 
        strncpy(kv_pairs[nth_pair].key, &json[start_index], (JSON_KEY_LEN < str_len ? JSON_KEY_LEN : str_len));

        // Add string termination character 
        kv_pairs[nth_pair].key[(JSON_KEY_LEN < str_len ? JSON_KEY_LEN : str_len)] = '\0';

        // Label key as stored 
        key_stored = true;

        // Continue to next token 
        continue;
      
      // Otherwise store value
      } else {
        // Copy token string into value 
        strncpy(kv_pairs[nth_pair].val, &json[start_index], (JSON_VAL_LEN < str_len ? JSON_VAL_LEN : str_len));

        // Add string termination character 
        kv_pairs[nth_pair].val[(JSON_KEY_LEN < str_len ? JSON_KEY_LEN : str_len)] = '\0';

        // Label key as missing 
        key_stored = false;

        // Update number of pairs 
        nth_pair++;

        // Continue to next token 
        continue;
      }
    }
  }

  // done. return number of key-value pairs found
  return nth_pair;
}

// Stringify key-value pairs into JSON string
int json_stringify(json_t *kv_pairs, size_t kv_pairs_len, char *json_str, size_t json_str_len) {
  // Check for null pointers 
  if(kv_pairs == NULL || json_str == NULL) {
    fprintf(stderr, "cannot take null pointer : ");
    return -1;
  }

  // Front JSON string with delimiting characters 
  json_str[0] = '{';

  // Stringify
  int json_str_x = 1;
  for(int x = 0; x < kv_pairs_len && json_str_x < json_str_len; x++) {
    // Create placeholder for next key-value pair
    char next_kv_str[JSON_KEY_LEN + JSON_VAL_LEN + 5];

    // Format string into placeholder 
    int next_kv_str_len = snprintf(next_kv_str, json_str_len - json_str_x, "\"%s\":\"%s\",", kv_pairs[x].key, kv_pairs[x].val);

    // Add next key-value pair string to the JSON string 
    strncpy(&json_str[json_str_x], next_kv_str, next_kv_str_len+1);

    // Update JSON string running length 
    json_str_x += next_kv_str_len;

    // Remove null termination character 
    // json_str[json_str_x] = ' ';
  }

  // Termination JSON string with delimiting characters 
  json_str[json_str_x] = '}';
  json_str_x++;
  json_str[json_str_x] = '\0';

  // done. return length of stringified JSON
  return json_str_x;
}

// Attenpts to parse string into JSON and reports if valid JSON or not.
// NOTE: Only supports up to JSON_TEST_LEN key-value pairs. Use json_ntest 
//       to set custom key-value pair limit.
bool json_test(char *json, size_t json_len) {
  // Create test placeholders 
   json_t kv_pairs[JSON_TEST_LEN];

   // Try 
   if(json_parse(json, json_len, kv_pairs, JSON_TEST_LEN) < 0) {
     return false;
   }

  // done, tests pass.
  return true;
}

// Attenpts to parse string into JSON and reports if valid JSON or not
// NOTE: This function uses <n> key-value pair limit to test parsing.
bool json_ntest(char *json, size_t json_len, size_t n) {
  // Create test placeholders 
   json_t kv_pairs[n];

   // Try 
   if(json_parse(json, json_len, kv_pairs, n) < 0) {
     return false;
   }

  // done, tests pass.
  return true;
}