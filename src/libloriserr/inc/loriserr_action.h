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

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_ACTION_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_ACTION_H

// Portal
#define DEFAULT_ACTION(f) DEFAULT_ACTION_##f

// Default actions
#define DEFAULT_ACTION_antenna_read_rs return -1

#endif