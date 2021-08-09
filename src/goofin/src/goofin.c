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

#include <stdio.h>
#include "loriserr.h"

#define STR(x) SUBSTR(x)
#define SUBSTR(x) #x

int main() {
  printf("str = %s\n",
         STR(LORISERR_FIX_CAT(
             LORISERR_ACTION_,
             LORISERR_FIX_RESOLVE(LORISERR_LVL_MOD | LORISERR_SUB_CORE))));

  return 0;
}