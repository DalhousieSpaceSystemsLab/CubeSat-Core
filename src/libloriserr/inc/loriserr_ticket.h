/**
 * @file loriserr_ticket.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Definition of the error ticket for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-05
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_TICKET_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_TICKET_H

#include <errno.h>
#include "time.h"

struct LORISERR_TICKET {
  int loris_errno;     // LORISERR error number
  time_t time;         // Time error was detected
  const char* f_call;  // Name of function call
  int c_errno;         // C errno
};

#endif