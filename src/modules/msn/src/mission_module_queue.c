/**
 * @file mission_module_queue.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Handles the queue for the mission module
 * @version 0.1
 * @date 2022-02-26
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "mission_module.h"

/**
 * @brief Add mission to queue
 *
 * @param msn
 * @return index if set, -1 otherwise
 */
int add_mission(struct mission mission_queue[static MAX_NUM_MISSIONS],
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
int rm_mission(int index,
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
int check_mission_queue(struct mission mission_queue[static MAX_NUM_MISSIONS]) {
  for (int x = 0; x < MAX_NUM_MISSIONS; x++) {
    // Check for incoming messages
    OK(ipc_refresh());

    // Unassigned mission
    if (mission_queue[x].cond_type == MISSION_UNASSIGNED) {
      continue;
    } else if (mission_queue[x].cond_type == MISSION_COND_GPS) {
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
      if (mission_queue[x].gps.min[0] <= lattitude &&
          lattitude <= mission_queue[x].gps.max[0] &&
          mission_queue[x].gps.min[1] <= longitude &&
          longitude <= mission_queue[x].gps.max[1]) {
        // Send command to command module
        OK(ipc_send_cmd(ipc.core.cmd.name, mission_queue[x].cmd));

        // Mission successfully executed, remove from queue
        OK(rm_mission(x, mission_queue));
      }
    } else if (mission_queue[x].cond_type == MISSION_COND_TIME) {
      // Check if current time is within margin
      time_t current_t = time(NULL);
      if (current_t > mission_queue[x].exe_time &&
          current_t < mission_queue[x].exe_time + TIME_COND_MARGIN) {
        // Send command to command module
        OK(ipc_send_cmd(ipc.core.cmd.name, mission_queue[x].cmd));

        // Mission successfully executed, remove from queue
        OK(rm_mission(x, mission_queue));
      }

      // Check if opporunity passed
      if (current_t > mission_queue[x].exe_time + TIME_COND_MARGIN) {
        // Remove mission
        OK(rm_mission(x, mission_queue));
      }
    } else if (mission_queue[x].cond_type == MISSION_COND_BOTH) {
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
