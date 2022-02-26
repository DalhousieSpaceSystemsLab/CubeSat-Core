#ifndef CUBESAT_CORE_MODULES_GPS_MODULE_H
#define CUBESAT_CORE_MODULES_GPS_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct gps_coord_rec {
  char gps_coord_lat[2667];
  char gps_coord_long[2667];
} gps_coord_rec;

void gps_coord_retreive(gps_coord_rec*, FILE*);

NEW_DOCK_MODULE(gps);

#endif