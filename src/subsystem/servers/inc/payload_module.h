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
#include "subsystem_module.h"
#include "ipc_std.h"

// Standard C libraries
#include <string.h>
#include <signal.h>

// Server container
extern SubsystemModule payload_server;

#endif 