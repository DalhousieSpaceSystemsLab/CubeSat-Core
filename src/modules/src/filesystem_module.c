/**
 * @file filesystem_module.c
 * @author Iftekhar Hossain (mdiftekharhr@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Dalhousie Space Systems Lab (c) 2021
 * 
 */

#include "filesystem_module.h"
;

// Callback general
CALLBACK (general) {
  
}

// Module functions
START_MODULE(filesystem_module) {
  OK(ipc_connect("fsm"));
  
}