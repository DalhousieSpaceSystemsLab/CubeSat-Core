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