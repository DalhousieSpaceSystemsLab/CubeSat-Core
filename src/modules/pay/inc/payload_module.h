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

NEW_DOCK_MODULE(payload);

#endif