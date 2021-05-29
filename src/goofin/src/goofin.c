/**
 * @file goofin.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Coding sandbox for this repo; just for goofin & testing
 * @version 0.1
 * @date 2021-05-28
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "jtok.h"

#include <stdio.h>

int main() {
  // Try parsing some JSON
  int tkns_len = 10;
  jtok_tkn_t tkns[10];
  if (jtok_parse("{\"first_k\":\"first_val\", \"second_k\":\"second_val\"}",
                 tkns, tkns_len) != JTOK_PARSE_STATUS_OK) {
    printf("parsing error\n");
    return -1;
  }

  for (int x = 0; x < tkns_len && tkns[x].type != JTOK_UNASSIGNED_TOKEN; x++) {
    printf("tkns[%d] = %.*s\n", x, tkns[x].end - tkns[x].start, tkns[x].json + tkns[x].start);
  }

  return 0;
}