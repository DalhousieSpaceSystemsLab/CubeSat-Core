/**
 * @file raven_module.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Raven telemetry support for LORIS
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "raven_module.h"

START_MODULE(rav) {
  // Open GPIO
  int gpio = open(GPIO_PATH, O_RDONLY);

  // Main loop
  for (;;) {
    // Get current time
    time_t start = time(NULL);

    // Open current logfile
    char datestamp[MAX_DATESTAMP_SIZE];
    struct tm* t = localtime(&start);
    strftime(datestamp, MAX_DATESTAMP_SIZE, "%F_%T", t);
    char filename[MAX_FILENAME_SIZE];
    sprintf(filename, "%s/%s.log", LOG_DIRECTORY, datestamp);
    FILE* fp = fopen(filename, "w");
    if (!fp) {
      moderr("Failed to create logfile for raven status\n");
      continue;
    }

    // Write datestamp at top of file
    fwrite(datestamp, sizeof(char), strlen(datestamp), fp);
    fputs("\n", fp);
    fflush(fp);

    time_t current = start;
    while (current < start + FILE_INTERVAL) {
      // Read from GPIO
      char val;
      read(gpio, &val, sizeof(char));

      // Log value
      fwrite(&val, sizeof(char), 1, fp);
      fputs("\n", fp);

      // Delay
      usleep(LOG_INTERVAL);

      // Update current time
      current = time(NULL);
    }

    // Write final datestamp at end of file
    t = localtime(&current);
    strftime(datestamp, MAX_DATESTAMP_SIZE, "%F_%T", t);
    fwrite(datestamp, sizeof(char), strlen(datestamp), fp);
    fflush(fp);

    // Close log file
    fclose(fp);
  }
}

STOP_MODULE(rav) {}

EXPORT_MODULE(rav);