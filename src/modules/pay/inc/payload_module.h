/**
 * payload_module.h
 *
 * purpose: handles operations related to the payload susbystem
 * author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_PAYLOAD
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVER_PAYLOAD

// Macros
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

// Project headers
// #include "ArducamDemo.h"
#include "subsysmod.h"

// Standard C libraries
#include <signal.h>
#include <string.h>

// Settings
#define CAMERA_BINARY_PATH "./payload/color_capture"

NEW_DOCK_MODULE(payload);

#endif

/**
 * root@colibri-imx6:~# ls payload/
color2_capture  color3_capture  color_capture  images  mono_capture
 *
 */