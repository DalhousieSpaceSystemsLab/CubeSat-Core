/**
 * @file loriserr_errno.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Stores the error numbers for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-05
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_ERRNO_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_ERRNO_H

//  Levels  //

enum {
  LORISERR_LVL_API = (1 << 0),
  LORISERR_LVL_MOD = (1 << 1),
  LORISERR_LVL_DCK = (1 << 2),
  LORISERR_LVL_SYS = (1 << 3),
  LORISERR_LVL_HRD = (1 << 4),
};
#define LORISERR_LVL_MAX LORISERR_LVL_HRD

//  Subsystems  //

enum {
  LORISERR_SUB_RF = (LORISERR_LVL_MAX << 1),
  LORISERR_SUB_ACS = (LORISERR_LVL_MAX << 2),
  LORISERR_SUB_PAY = (LORISERR_LVL_MAX << 3),
  LORISERR_SUB_POW = (LORISERR_LVL_MAX << 4),
  LORISERR_SUB_GPS = (LORISERR_LVL_MAX << 5),
  LORISERR_SUB_GEN = (LORISERR_LVL_MAX << 6),
  LORISERR_SUB_CORE = (LORISERR_LVL_MAX << 7),
};
#define LORISERR_SUB_MAX LORISERR_SUB_CORE

//  Types  //

// General
enum {
  LORISERR_T_NULL_PTR = (LORISERR_SUB_MAX << 1),
  LORISERR_T_FOPEN_FAIL = (LORISERR_SUB_MAX << 2),
  LORISERR_T_MALLOC_FAIL = (LORISERR_SUB_MAX << 3),
};

#endif