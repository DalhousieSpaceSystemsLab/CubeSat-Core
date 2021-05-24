/**
 * @file modutil.h
 * @author alex amellal (loris@alexamellal.com)
 * @brief Provide useful utilities for modules
 * @version 0.1
 * @date 2021-04-15
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef CUBESAT_CORE_LIBIPC_MODUTIL_H
#define CUBESAT_CORE_LIBIPC_MODUTIL_H

// Standard C libraries
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Wrapper for printf which outputs a message to stdout while identifying
 * the module calling it.
 *
 * @param func_name Name of the function of origin for the call. See MODPRINTF
 * for automation.
 * @param msg Contains message to be printed to stdout (may contain printf %d
 * and %s etc)
 * @param ... printf variables which go with %d and %s etc
 */
void modfprintf(const char* func_name, FILE* stream, const char* msg, ...);
#define modprintf(msg, ...) modfprintf(__FUNCTION__, stdout, msg, ##__VA_ARGS__)
#define moderr(msg, ...) modfprintf(__FUNCTION__, stderr, msg, ##__VA_ARGS__)
#define dockprintf(msg, ...) modfprintf("DOCK", stdout, msg, ##__VA_ARGS__)
#define dockerr(msg, ...) modfprintf("DOCK", stderr, msg, ##__VA_ARGS__)

// Attempts to stop process using interrupt signal within timeout
// If timeout exceeded and process has not terminated, SIGKILL is sent.
static void fstop(pid_t pid, int sec_timeout, struct timespec retry_delay);

#define OK(func)                  \
  if (func < 0) {                 \
    moderr("%s failed\n", #func); \
    return -1;                    \
  }

// Timeout
int twaitpid(pid_t pid, int* status, int timeout);
#endif