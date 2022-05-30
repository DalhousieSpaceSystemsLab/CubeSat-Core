#include "burnwire_module.h"

// Glocal variables

static int trigger_burn();
static int check_lockfile();

CALLBACK(listen_trigger) {
  // Check command
  if (strcmp(msg, ipc.brn.cmd.trigger_burn) == 0) {
    // Trigger burn
    trigger_burn();
  }
  STOP_CALLBACK;
}

START_MODULE(brn) {
  // Upon startup, check for lockfile
  check_lockfile();

  // Connect to IPC
  OK(ipc_connect(ipc.brn.name));

  // Create listener for trigger
  OK(ipc_create_listener("*", listen_trigger, NULL));

  // Main loop
  for (;;) {
    OK(ipc_refresh());
  }
}

STOP_MODULE(brn) {}

static int trigger_burn() {
  system(BURNWIRE_COMMAND);
  // done
  return 0;
}

static int check_lockfile() {
  // Check if lockfile missing
  if (access(BURNWIRE_LOCK, F_OK) == -1) {
    trigger_burn();
  }

  // done
  return 0;
}

EXPORT_MODULE(brn);
