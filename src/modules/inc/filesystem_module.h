/**
 * filesystem_module.h
 *
 * purpose: Stores, retrieves and organizes files on the
 *          satellite's filesystem.
 * author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_FILESYSTEM_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_FILESYSTEM_H

// Macros
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

// Project headers
#include "ipc_std.h"
#include "subsysmod.h"

// Standard C libraries
#include <signal.h>
#include <string.h>

// Subsystem module
extern SubsystemModule filesystem_module;

#endif