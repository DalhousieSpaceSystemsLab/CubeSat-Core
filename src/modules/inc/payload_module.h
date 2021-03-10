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
#define _XOPEN_SOURCE

// Project headers
#include "ipc_std.h"
#include "subsystem_module.h"

// Standard C libraries
#include <signal.h>
#include <string.h>

// Server container
extern SubsystemModule payload_module;

#endif