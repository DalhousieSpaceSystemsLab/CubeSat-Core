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

START_MODULE(test_module) {
   OK(ipc_connect("tst"));
   //send a message
   char msg[] = "Testing for IPC connection";
   OK(ipc_send("gps", msg, strlen(msg)));
   sleep(1000);
}
 
STOP_MODULE(test_module) {
 OK(ipc_disconnect());


}

EXPORT_MODULE(test_module);
