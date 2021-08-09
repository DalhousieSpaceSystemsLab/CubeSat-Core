/**
 * @file loriserr_fix.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Error correction for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-11
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_FIX_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_FIX_H

#define LORISERR_FIX_RESOLVE(x) \
  ({                            \
    int _x_ = x;                \
    _x_;                        \
  })
#define LORISERR_FIX_CAT(a, b) LORISERR_FIX_SUBCAT(a, b)
#define LORISERR_FIX_SUBCAT(a, b) a##b

#endif