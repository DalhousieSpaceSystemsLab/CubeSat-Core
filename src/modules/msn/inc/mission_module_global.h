/**
 * @file mission_module_settings.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Global settings header for mission module
 * @version 0.1
 * @date 2022-02-26
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_MISSION_MODULE_GLOBAL_H
#define CUBESAT_CORE_MODULES_MISSION_MODULE_GLOBAL_H

// Project headers
#include "subsysmod.h"

/// Settings
#define MAX_NUM_MISSIONS 10
#define TIME_COND_MARGIN 30  // in seconds

/// Mission execution condition types
enum {
  MISSION_UNASSIGNED = 0,
  MISSION_COND_GPS,
  MISSION_COND_TIME,
  MISSION_COND_BOTH,
};

/// GPS struct
struct gps_coor {
  float min[2];
  float max[2];
};

/// Mission struct
struct mission {
  int cond_type;          // See mission types
  struct gps_coor gps;    // GPS coordinate range
  time_t exe_time;        // C time if mission is time type
  char cmd[MAX_MSG_LEN];  // Command to be sent to dest
};

#endif