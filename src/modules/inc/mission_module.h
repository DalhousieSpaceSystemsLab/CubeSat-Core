#ifndef CUBESAT_CORE_MODULES_MISSION_MODULE_H
#define CUBESAT_CORE_MODULES_MISSION_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C libraries
#include <stdio.h>
#include <unistd.h>

/// Settings
#define MAX_NUM_MISSIONS 10

/// Mission execution condition types
enum { MISSION_UNASSIGNED = 0, MISSION_COND_GPS, MISSION_COND_TIME };

/// Mission struct
struct mission {
  int cond_type;          // See mission types
  float gps_coor_min[2];  // GPS coordinates if mission is gps type (lat, long)
  float gps_coor_max[2];  // GPS coordinates if mission is gps type (lat, long)
  time_t exe_time;        // C time if mission is time type
  char cmd[MAX_MSG_LEN];  // Command to be sent to dest
};

NEW_DOCK_MODULE(mission);

#endif