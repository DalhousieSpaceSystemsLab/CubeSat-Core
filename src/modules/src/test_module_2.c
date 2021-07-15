/**
 * @file test_module_2.c
 * @author Iftekhar Hossain (mdiftekharhr@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-29
 * 
 * @copyright Dalhousie Space Systems Lab (c) 2021
 * 
 */


#include "test_module_2.h"
;

START_MODULE(test_module_2) {
   OK(ipc_connect("tvt"));
   //send a message
   char msg[] = "Testing for IPC connection";
   OK(ipc_send("tst", msg, strlen(msg)));
   sleep(30);
}
 


STOP_MODULE(test_module_2) {
 OK(ipc_disconnect());


}

EXPORT_MODULE(test_module_2);