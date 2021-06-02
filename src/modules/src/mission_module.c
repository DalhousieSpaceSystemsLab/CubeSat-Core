#include "mission_module.h"

/// Define GPS coordinate range
static float range[4] = {10.0f, 10.0f, 45.0f, 45.0f};

/// Mission queue
static struct mission missions[MAX_NUM_MISSIONS];

static int check_mission_queue() {
  for (int x = 0; x < MAX_NUM_MISSIONS; x++) {
  }
}

// Handle incoming messages from command module
CALLBACK(command) {
  // Get args
  int argc = ipc_get_n_args(msg, msg_len);
  char args[argc][MAX_ARG_LEN];
  OK(ipc_get_args(msg, msg_len, args, argc));

  // Check command -- Queue mission with GPS conditions
  if (ipc_check_cmd(msg, "%s %s", ipc.core.msn.cmd.qmsn, "gps")) {
    // Check argc
    if (argc != 4) {
      moderr(
          "Invalid number of arguments for qmsn with gps coordinates. "
          "SKIPPING\n");
      STOP_CALLBACK;
    }

    // Get lattitude and longitude
    float lattitude = atof(args[2]);
    float longitude = atof(args[3]);

    // Check if in range
    if (range[0] <= lattitude && lattitude <= range[2] &&
        range[1] <= longitude && longitude <= range[3]) {
      modprintf("We are in range\n");

      // Send message to payload
      OK(ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic));
    }
  } else {
    modprintf("wah wah didnt match\n");
  }

  STOP_CALLBACK;
}

// Handle general incoming messages
CALLBACK(general) {
  modprintf("Incoming msg: %s\n", msg);

  STOP_CALLBACK;
}

START_MODULE(mission) {
  OK(ipc_connect(ipc.core.msn.name))

  // Set up listeners
  OK(ipc_create_listener(ipc.core.cmd.name, command, NULL));
  OK(ipc_create_listener("*", general, NULL));

  // Refresh incoming messages
  for (;;) {
    OK(ipc_refresh());
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);