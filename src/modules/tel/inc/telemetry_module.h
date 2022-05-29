#ifndef CUBESAT_CORE_MODULES_TEL_INC_TELEMETRY_MODULE_H
#define CUBESAT_CORE_MODULES_TEL_INC_TELEMETRY_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C libs
#include <stdio.h>
#include <stdlib.h>

// Settings
#define TELEMETRY_INTERVAL 60       // in seconds
#define LARGE_TEL_LINES_KEEP "100"  // in lines
#define OP_DELAY 500000             // in microseconds
#define SMALL_TEL_FILE "telemetry-small.txt"
#define LARGE_TEL_FILE "telemetry-large.txt"
#define SHAVE_TEL_FILE "telemetry-shaved.txt"

// Commands
#define CPU_TEMP_COMMAND "cat /sys/devices/virtual/thermal/thermal_zone0/temp"
#define FILESYSTEM_COMMAND "df -h"
#define DATESTAMP_COMMAND "date"
#define UPTIME_COMMAND "uptime"
#define ECHO_COMMAND "echo"
#define CAT_COMMAND "cat"
#define CPU0_FREQ_COMMAND \
  "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"
#define CPU1_FREQ_COMMAND \
  "cat /sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_cur_freq"
#define TAIL_COMMAND "tail -n"

NEW_DOCK_MODULE(tel);

#endif
