/**
 * @file loriserr_action.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Defines the action struct for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-11
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include <unistd.h>

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_ACTION_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_ACTION_H

#define LORISERR_ACTION_NONE return 0
#define LORISERR_ACTION_WAIT(t) sleep(t)

#endif