/**
 * payload.c
 *
 * purpose: handles operations related to the payload susbystem
 * author:  alex amellal
 *
 */

#include "payload_module.h"

CALLBACK(payload_general) {
  // Take pic
  if (strncmp(msg, ipc.pay.cmd.take_pic, msg_len) == 0) {
    modprintf("taking picture...\n");

    char **argv = NULL;
    // main_f(0, argv);

    modprintf("done!\n");
  } else {
    modprintf("misc message incoming: %.*s\n", msg_len, msg);
  }

  // done
  STOP_CALLBACK;
}

START_MODULE(payload) {
  // Connect to the IPC
  OK(ipc_connect(ipc.pay.name))

  // Create listener for general requests
  OK(ipc_qrecv("*", payload_general, NULL, IPC_QRECV_MSG))

  // Keep refreshing incoming messages
  for (;;) {
    OK(ipc_refresh())
  }
}

STOP_MODULE(payload) {
  // Disconnect from the IPC
  ipc_disconnect();
  modprintf("disconnecting payload\n");
}

EXPORT_MODULE(payload);