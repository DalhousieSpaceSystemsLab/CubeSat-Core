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

#include <errno.h>
#include <stdio.h>
#include "loriserr_det.h"

int main() {
  struct LORISERR_TICKET tkt = LORISERR_DET_CTX(
      LORISERR_LVL_API | LORISERR_SUB_GEN | LORISERR_T_MALLOC_FAIL, time(NULL),
      __FUNCTION__, errno);
  LORISERR_DET_REPORT(tkt);
  return 0;
}