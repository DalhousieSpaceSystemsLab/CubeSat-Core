//server.h

#ifndef CUBESAT_CORE_INCLUDE_IPC_SERVER_H
#define CUBESAT_CORE_INCLUDE_IPC_SERVER_H

//Project Headers
#include "client_api.h"

//Methods
int server_init(); //initialize server by connecting to IPC
void receiving(); //Receives message from client
void formatting(buffer, fmt_buffer); //Formats the message into map format
void do_msn(fmt_buffer); //Analyses and start mission

#endif
