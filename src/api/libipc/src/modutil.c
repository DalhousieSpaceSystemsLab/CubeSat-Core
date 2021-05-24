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

#include "modutil.h"

void modfprintf(const char* func_name, FILE* stream, const char* msg, ...) {
  // Init variadic arg
  va_list ap;
  va_start(ap, msg);

  // Print identifier
  char notif = 'i';
  if (stream != stderr) {
    stream = stdout;
  } else {
    notif = '!';
  }

  fprintf(stream, "[%c](%s) ", notif, func_name);
  vfprintf(stream, msg, ap);
  fprintf(stream, "\n");
  fflush(stream);

  // Cleanup
  va_end(ap);

  // done
}

/**
 * @brief Implements timeout feature in waitpid method
 *
 * @param pid Process ID of process in question
 * @param status If not NULL, exit status of process will be stored here.
 * @param timeout Max time spent waiting for process to stop in seconds.
 */
int twaitpid(pid_t pid, int* status, int timeout) {
  // Create plceholders for time trackers
  time_t start, current, time_elapsed = 0;

  // Set start time
  time(&start);

  // Keep waitpid-no-hangin' until time runs out or process exits
  while (time_elapsed < timeout) {
    // Process has not yet changed state
    if (waitpid(pid, status, WNOHANG) == 0) {
      // Update current time
      time(&current);
      time_elapsed = current - start;
      continue;
    } else {
      // Process has exited, we can stop
      return 1;
    }
  }

  // done
  // process did not exit
  return 0;
}

/**
 * @brief Attempts to stop process using interrupt signal within timeout.
 * If timeout exceeded and process has not terminated, SIGKILL is sent.
 *
 * @param pid PID of process to stop.
 * @param sec_timeout Timeout given to process until sending SIGKILL
 * @param retry_delay Delay between waitpid attempts
 */
static void fstop(pid_t pid, int sec_timeout, struct timespec retry_delay) {
  bool pexited = false, psigterm = false;
  time_t start, current, time_elapsed = 0;
  time(&start);
  while (time_elapsed < sec_timeout) {
    kill(pid, SIGINT);
    int stat;
    waitpid(pid, &stat, WNOHANG);

    pexited = WIFEXITED(stat);
    psigterm = WIFSIGNALED(stat);
    if (pexited || psigterm) break;

    nanosleep(&retry_delay, NULL);
    time(&current);
    time_elapsed = current - start;
  }

  // Check to see if start process needs to be force stopped
  if (!(pexited || psigterm)) {
    kill(pid, SIGKILL);
  }
}