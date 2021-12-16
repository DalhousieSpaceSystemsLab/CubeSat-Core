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

#define LORISERR_FIX_ACTION(action) LORISERR_FIX_ACTION_##action

#define LORISERR_FIX_ACTION_DO_NOTHING 0

LORISERR_FIX_ACTION(DO_NOTHING)

#endif