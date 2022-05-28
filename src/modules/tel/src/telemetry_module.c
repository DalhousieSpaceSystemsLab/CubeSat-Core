
#include "telemetry_module.h"

// Glocal variables
static FILE* small_fp = NULL;
static FILE* large_fp = NULL;

static int telemetry_gen_small();
static int telemetry_gen_large();

START_MODULE(tel) {
  // Conect to IPC
  OK(ipc_connect("tel"));

  // Main loop
  for (;;) {
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
