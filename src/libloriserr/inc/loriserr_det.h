/**
 * @file loriserr_det.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Error detection for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-05
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBLORISERR_LORISERR_DET_H
#define CUBESAT_CORE_LIBLORISERR_LORISERR_DET_H

// Project headers
#include "loriserr_errno.h"
#include "loriserr_ticket.h"

// Standard C Libraries
#include <stdio.h>
#include <sys/time.h>

/**
 * @brief Generates a ticket using context
 *
 * @param loris_errno LORIS error number
 * @param t Time error was detected
 * @param f_call Name of function call
 * @param c_errno C error number
 * @return struct LORISERR_TICKET
 */
struct LORISERR_TICKET LORISERR_DET_CTX(int loris_errno, time_t t,
                                        const char* f_call, int c_errno);

/**
 * @brief Reports an error if the ticket flags an error
 *
 * @param ticket
 */
void LORISERR_DET_REPORT(struct LORISERR_TICKET ticket);

#endif