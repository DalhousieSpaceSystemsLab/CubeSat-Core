/**
 * @file test_module.c
 * @author Iftekhar Hossain (mdiftekharhr@gmail.com)
 * @brief Test module
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Dalhousie Space Systems Lab (c) 2021
 * 
 */

#include "test_module.h"
;

CALLBACK (tvt) {
  modprintf("Your message has been recieved: %c", msg);


}
START_MODULE(test_module) {

  OK(ipc_connect("tst"));

  char TEST[MAX_MSG_LEN];

   OK(ipc_create_listener("*", tvt, MAX_MSG_LEN));
  /*
  OK(ipc_recv("tvt", TEST, MAX_MSG_LEN, NO_TIMEOUT ));

  modprintf("Message has been recieved.");
  */
 //Refreshing message status
 for (;;) {
    OK(ipc_refresh())
  }

  sleep(30);
  }


STOP_MODULE(test_module) {
 OK(ipc_disconnect());


}

EXPORT_MODULE(test_module);
