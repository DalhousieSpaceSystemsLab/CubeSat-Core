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
#include "ArducamDemo.h"
#include "subsysmod.h"
// #include "capture.h"

// Standard C libraries
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

NEW_DOCK_MODULE(payload);

#endif