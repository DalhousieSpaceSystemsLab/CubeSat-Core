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