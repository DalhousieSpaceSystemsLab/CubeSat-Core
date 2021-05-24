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

// CONSTS
#define FSTOP_TIMEOUT 1
#define FSTOP_ATT_DELAY \
  (struct timespec) { .tv_sec = 0, .tv_nsec = 100000000l }

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
void fstop(pid_t pid, int sec_timeout, struct timespec retry_delay);

/**
 * @brief Macro used to check return value of function call.
 * If the return value is less than 0, it will output and error using
 * moderr with the function name, and return -1 to the parent.
 *
 */
#define OK(func)                  \
  if (func < 0) {                 \
    moderr("%s failed\n", #func); \
    return -1;                    \
  }

/**
 * @brief Implements timeout feature in waitpid method
 *
 * @param pid Process ID of process in question
 * @param status If not NULL, exit status of process will be stored here.
 * @param timeout Max time spent waiting for process to stop in seconds.
 *
 * @return 0 = process did exit, 1 = process timed out OR error
 */
int twaitpid(pid_t pid, int* status, int timeout);
#define TIMEOUT(_func, _wait)                    \
  {                                              \
    pid_t child_stat = fork();                   \
    if (child_stat == 0) {                       \
      _func;                                     \
    } else {                                     \
      twaitpid(child_stat, NULL, _wait) ? 1 : 0; \
    }                                            \
  }

#define FTIMEOUT(_func, _wait)                      \
  {                                                 \
    pid_t child_stat = fork();                      \
    if (child_stat == 0) {                          \
      _func;                                        \
    } else {                                        \
      if (twaitpid(child_stat, NULL, _wait)) {      \
        fstop(pid, FSTOP_TIMEOUT, FSTOP_ATT_DELAY); \
      }                                             \
    }                                               \
  }

#endif