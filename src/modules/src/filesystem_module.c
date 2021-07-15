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
  char TEST[MAX_MSG_LEN];

  OK(ipc_create_listener("*", general, MAX_MSG_LEN));

  //Refreshing message status
  for(;;){
    OK(ipc_refresh());
  }
}

STOP_MODULE(filesystem_module){
  OK(ipc_disconnect());
}
