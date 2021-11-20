/**
 * @file loriserr.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Main header for the LORIS Automated Error Handling Library
 * @version 0.1
 * @date 2021-07-05
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_H

#include "loriserr_det.h"
#include "loriserr_fix.h"

//  Macros  //

#define LORISERR_DET(val)                                       \
  ({                                                            \
    struct LORISERR_TICKET _loriserr_tkt_ =                     \
        LORISERR_DET_CTX(val, time(NULL), __FUNCTION__, errno); \
    LORISERR_DET_REPORT(_loriserr_tkt_);                        \
    _loriserr_tkt_;                                             \
  })
#define LORISERR_FIX(func, ctx) 0
#define LORISERR_RESET(state) 0

#endif