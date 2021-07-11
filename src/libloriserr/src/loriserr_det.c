/**
 * @file loriserr_det.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Error detection for the LORIS AEH library
 * @version 0.1
 * @date 2021-07-05
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "loriserr_det.h"

// Generates a ticket using context
struct LORISERR_TICKET LORISERR_DET_CTX(int loris_errno, time_t t,
                                        const char* f_call, int c_errno) {
  return (struct LORISERR_TICKET){
      .loris_errno = loris_errno,
      .time = t,
      .f_call = f_call,
      .c_errno = errno,
  };
}

// Reports an error if the ticket flags an error
void LORISERR_DET_REPORT(struct LORISERR_TICKET ticket) {
  // Check if the ticket contains an error
  if (ticket.loris_errno) {
    // Get current time
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* tm_info = localtime(tv.tv_sec);

    // Format date and time
    char datentime[24];
    strftime(datentime, 24, "%Y-%m-%d %H:%M:%S", tm_info);

    // Format timestamp
    char timestamp[32];
    sprintf(timestamp, "(%s.%ld) ", datentime, tv.tv_usec);

    // Print timestamp
    fprintf(stderr, timestamp);

    // Report according to level
    if (ticket.loris_errno & LORISERR_LVL_API) {
      fprintf(stderr, "[API ->");
    } else if (ticket.loris_errno & LORISERR_LVL_MOD) {
      fprintf(stderr, "[MOD ->");
    } else if (ticket.loris_errno & LORISERR_LVL_DCK) {
      fprintf(stderr, "[DCK ->");
    } else if (ticket.loris_errno & LORISERR_LVL_SYS) {
      fprintf(stderr, "[SYS ->");
    } else if (ticket.loris_errno & LORISERR_LVL_HRD) {
      fprintf(stderr, "[HRD ->");
    }

    // Report subsystem
    if (ticket.loris_errno & LORISERR_SUB_RF) {
      fprintf(stderr, "RF -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_ACS) {
      fprintf(stderr, "ACS -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_PAY) {
      fprintf(stderr, "PAY -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_POW) {
      fprintf(stderr, "POW -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_GPS) {
      fprintf(stderr, "GPS -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_GEN) {
      fprintf(stderr, "GEN -> ");
    } else if (ticket.loris_errno & LORISERR_SUB_CORE) {
      fprintf(stderr, "CORE -> ");
    }

    // Report type
    if (ticket.loris_errno & LORISERR_T_NULL_PTR) {
      fprintf(stderr, "NULL_PTR] ");
    } else if (ticket.loris_errno & LORISERR_T_FOPEN_FAIL) {
      fprintf(stderr, "FOPEN_FAIL] ");
    } else if (ticket.loris_errno & LORISERR_T_MALLOC_FAIL) {
      fprintf(stderr, "MALLOC_FAIL] ");
    }
  }
}