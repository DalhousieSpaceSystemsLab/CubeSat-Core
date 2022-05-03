#include "mission_module.h"

/// Define GPS coordinate range
static float range[4] = {10.0f, 10.0f, 45.0f, 45.0f};

/// Mission queue
static struct mission missions[MAX_NUM_MISSIONS] = {{0}};

// Handle incoming messages from command module
CALLBACK(command) {
  // Get args
  int argc = ipc_get_n_args(msg, msg_len);
  char args[argc][MAX_ARG_LEN];
  OK(ipc_get_args(msg, msg_len, args, argc));

  // Check command -- Queue mission with GPS conditions
  if (ipc_check_cmd(msg, "%s %s", ipc.core.msn.cmd.qmsn, "gps")) {
    // Check argc
    if (argc != 7) {
      moderr(
          "Invalid number of arguments for qmsn with gps coordinates. "
          "SKIPPING\n");
      STOP_CALLBACK;
    }

    // Get lattitude and longitude
    float gps_min[2], gps_max[2];
    gps_min[0] = atof(args[3]);
    gps_min[1] = atof(args[4]);
    gps_max[0] = atof(args[5]);
    gps_max[1] = atof(args[6]);

    // Add mission to queue
    struct mission msn = {
        .cond_type = MISSION_COND_GPS,
        .gps.min[0] = gps_min[0],
        .gps.min[1] = gps_min[1],
        .gps.max[0] = gps_max[0],
        .gps.max[1] = gps_max[1],
    };
    strcpy(msn.cmd, args[2]);

    ON_FAIL(add_mission(missions, msn),
            moderr("Cannot add mission to queue. SKIPPING.\n"));

  } else if (ipc_check_cmd(msg, "%s %s", ipc.core.msn.cmd.qmsn, "time")) {
    // Check args
    if (argc != 4) {
      moderr(
          "Invalid number of arguments for qmsn with time. "
          "SKIPPING\n");
      STOP_CALLBACK;
    }

    // Get time
    time_t t = atol(args[3]);

    // Add mission to queue
    struct mission msn = {
        .cond_type = MISSION_COND_TIME,
        .exe_time = t,
    };
    strcpy(msn.cmd, args[2]);

    ON_FAIL(add_mission(missions, msn),
            moderr("Cannot add mission to the queue. SKIPPING.\n"));
  } else {
    moderr("wah wah didnt match\n");
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

  // Refresh incoming messages & check missions
  for (;;) {
    OK(check_mission_queue(missions));
    OK(ipc_refresh());
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);