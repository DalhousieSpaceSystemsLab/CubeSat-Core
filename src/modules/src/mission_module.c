#include "mission_module.h"

/// Define GPS coordinate range
static float range[4] = {10.0f, 10.0f, 45.0f, 45.0f};

/// Mission queue
static struct mission missions[MAX_NUM_MISSIONS];

static int check_mission_queue() {
  for (int x = 0; x < MAX_NUM_MISSIONS; x++) {
    // Unassigned mission
    if (missions[x].cond_type == MISSION_UNASSIGNED) {
      continue;
    } else if (missions[x].cond_type == MISSION_COND_GPS) {
      // Get GPS coordinates
      int gps_coor_len = 0;
      char gps_coor[32];
      OK((gps_coor_len = ipc_send_cmd(ipc.gps.name, ipc.gps.cmd.get_cur_pos)));
      IF_TIMEOUT(ipc_recv(ipc.gps.name, gps_coor, 32, RECV_TIMEOUT), x--;
                 continue);

      // Convert into gps args
      char args[2][MAX_ARG_LEN];
      OK(ipc_get_args(gps_coor, gps_coor_len, args, 2));

      // Convert into floats
      float lattitude = atof(args[0]);
      float longitude = atof(args[1]);

      // Check if within range
      if (missions[x].gps_coor_min[0] <= lattitude &&
          lattitude <= missions[x].gps_coor_max[0] &&
          missions[x].gps_coor_min[1] <= longitude &&
          longitude <= missions[x].gps_coor_max[1]) {
        // Send command to destination module
        OK(ipc_send_cmd(missions[x].dest, missions[x].cmd));
      }
    } else if (missions[x].cond_type == MISSION_COND_TIME) {
      // Add time mission to queue
    } else {
      // Skip
      continue;
    }
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

      // Add mission to queue

      // Send command to payload
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