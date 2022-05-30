#include "telemetry_module.h"

// Glocal variables
static FILE* small_fp = NULL;
static FILE* large_fp = NULL;

static int telemetry_gen_small();
static int telemetry_gen_large();

START_MODULE(tel) {
  // Main loop
  for (char x = 0;; x++) {
    ON_FAIL_LOG(telemetry_gen_small(), 1;);
    ON_FAIL_LOG(telemetry_gen_large(), 1;);
    sleep(TELEMETRY_INTERVAL);
  }
}

STOP_MODULE(tel) {
  // Close files
  if (small_fp) fclose(small_fp);
  if (large_fp) fclose(large_fp);
}

static int telemetry_gen_small() {
  // Date + uptime
  system(ECHO_COMMAND " `date` '::' `uptime` > " SMALL_TEL_FILE);

  // done
  return 0;
}

static int telemetry_gen_large() {
  // Date & time
  system(
      ECHO_COMMAND
      " '################################################' >> " LARGE_TEL_FILE);
  system(DATESTAMP_COMMAND " >> " LARGE_TEL_FILE);
  system(
      ECHO_COMMAND
      " '################################################' >> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  // Filesystem details
  system(FILESYSTEM_COMMAND " >> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  // CPU temp
  system(ECHO_COMMAND
         " '~~~~~~~~~~ CPU_TEMPERATURE (in 1/1000deg C) ~~~~~~~~~~' "
         ">> " LARGE_TEL_FILE);
  system(CPU_TEMP_COMMAND " >> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  // CPU freq
  system(ECHO_COMMAND
         " '~~~~~~~~~~ CPU0 FREQUENCY (in kHz) ~~~~~~~~~~' "
         ">> " LARGE_TEL_FILE);
  system(CPU0_FREQ_COMMAND " >> " LARGE_TEL_FILE);
  system(ECHO_COMMAND
         " '~~~~~~~~~~ CPU1 FREQUENCY (in kHz) ~~~~~~~~~~' "
         ">> " LARGE_TEL_FILE);
  system(CPU1_FREQ_COMMAND " >> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  // Uptime
  system(ECHO_COMMAND
         " '~~~~~~~~~~ OBC UP-TIME ~~~~~~~~~~' "
         ">> " LARGE_TEL_FILE);
  system(UPTIME_COMMAND " >> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  system(ECHO_COMMAND
         " '######################## DONE #####################' "
         ">> " LARGE_TEL_FILE);
  usleep(OP_DELAY);

  // Shuffle large telemetry to only contain last 100 lines
  system(TAIL_COMMAND " " LARGE_TEL_LINES_KEEP " " LARGE_TEL_FILE
                      " > " SHAVE_TEL_FILE);
  system(CAT_COMMAND " " SHAVE_TEL_FILE " > " LARGE_TEL_FILE);

  // done
  return 0;
}

EXPORT_MODULE(tel);
