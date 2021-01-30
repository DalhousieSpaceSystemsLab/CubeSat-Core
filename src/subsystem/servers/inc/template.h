/**
 * template.h
 * 
 * purpose: demonstrate configuration of a subsystem server 
 * author: alex amellal 
 * 
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_TEMPLATE_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_TEMPLATE_H

// Project headers
#include "server_container.h"

// Feature test macros 
#define _XOPEN_SOURCE 700

// Standard C libraries 
#include <pthread.h>
#include <signal.h>

#include <stdio.h>
#include <unistd.h>

extern ServerContainer template_server;

#endif  // End of CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_TEMPLATE_H guard