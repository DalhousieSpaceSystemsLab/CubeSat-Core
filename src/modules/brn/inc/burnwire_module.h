#ifndef CUBESAT_CORE_MODULES_BRN_INCLUDE_BURNWIRE_MODULE_H
#define CUBESAT_CORE_MODULES_BRN_INCLUDE_BURNWIRE_MODULE_H

// Project headers
#include "subsysmod.h"

// Standard C libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Settings
#define BURNWIRE_COMMAND "./burnwire-test.sh"
#define BURNWIRE_LOCK "burnwire.lock"

NEW_DOCK_MODULE(brn);

#endif
