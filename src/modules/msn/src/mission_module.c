#include "mission_module.h"

/// Define GPS coordinate range
static float range[4] = {10.0f, 10.0f, 45.0f, 45.0f};

/// Mission queue
static struct mission missions[MAX_NUM_MISSIONS] = {{0}};

/**
 * @brief Add mission to queue
 *
 * @param msn
 * @return index if set, -1 otherwise
 */
static int add_mission(struct mission mission_queue[static MAX_NUM_MISSIONS],
                       struct mission msn) {
  int index = -1;
  for (int x = 0; x < MAX_NUM_MISSIONS; x++) {
    if (mission_queue[x].cond_type == MISSION_UNASSIGNED) {
      mission_queue[x] = msn;
      if (x + 1 < MAX_NUM_MISSIONS) {
        mission_queue[x + 1] = (struct mission){0};
      }
      index = x;
      break;
    }
  }

  return index;
}

/**
 * @brief Deletes the mission in the n'th position of the mission queue
 *
 * @param index
 * @param mission_queue
 * @return int
 */
static int rm_mission(int index,
                      struct mission mission_queue[static MAX_NUM_MISSIONS]) {
  if (index < 0) {
    return -1;
  }

  for (int x = index; (x + 1) < MAX_NUM_MISSIONS; x++) {
    mission_queue[x] = mission_queue[x + 1];
  }
  mission_queue[MAX_NUM_MISSIONS - 1] = (struct mission){0};

  return 0;
}

/**
 * @brief Iterates though queued missions and checks to see if their conditions
 * have been met
 *
 * @return int
 */
static int check_mission_queue(
    struct mission mission_queue[static MAX_NUM_MISSIONS]) {
  for (int x = 0; x < MAX_NUM_MISSIONS; x++) {
    // Check for incoming messages
    OK(ipc_refresh());

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
        // Send command to command module
        OK(ipc_send_cmd(ipc.core.cmd.name, missions[x].cmd));

        // Mission successfully executed, remove from queue
        OK(rm_mission(x, missions));
      }
    } else if (missions[x].cond_type == MISSION_COND_TIME) {
      // Check if current time is within margin
      time_t current_t = time(NULL);
      if (current_t > mission_queue[x].exe_time &&
          current_t < mission_queue[x].exe_time + TIME_COND_MARGIN) {
        // Send command to command module
        OK(ipc_send_cmd(ipc.core.cmd.name, missions[x].cmd));

        // Mission successfully executed, remove from queue
        OK(rm_mission(x, missions));
      }

      // Check if opporunity passed
      if (current_t > mission_queue[x].exe_time + TIME_COND_MARGIN) {
        // Remove mission
        OK(rm_mission(x, mission_queue));
      }
    } else if (missions[x].cond_type == MISSION_COND_BOTH) {
      // Placeholders for conditions
      bool time_matches = false;
      bool gps_matches = false;

      // Check if current time is within margin
      time_t current_t = time(NULL);
      if (current_t > mission_queue[x].exe_time &&
          current_t < mission_queue[x].exe_time + TIME_COND_MARGIN) {
        time_matches = true;
      }

      // Get GPS coordinates
      OK(ipc_send_cmd(ipc.gps.name, ipc.gps.cmd.get_cur_pos))

      // Check if current coordinates are within margin

    } else {
      // Skip
      continue;
    }
  }

  // done
  return 0;
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
        .gps_coor_min[0] = gps_min[0],
        .gps_coor_min[1] = gps_min[1],
        .gps_coor_max[0] = gps_max[0],
        .gps_coor_max[1] = gps_max[1],
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