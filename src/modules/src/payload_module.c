/**
 * payload.c
 *
 * purpose: handles operations related to the payload susbystem
 * author:  alex amellal
 *
 */

#include "payload_module.h"
#include "client_api.h"

CALLBACK(general) {
  // Take pic
  if (strncmp(msg, ipc.pay.cmd.take_pic, msg_len) == 0) {
    modprintf("taking picture...\n");
    modprintf("done!\n");
  } else {
    modprintf("misc message incoming: %.*s\n", msg_len, msg);
  }

  OK(ipc_send_cmd(ipc.core.msn.name, "waddup"))
}

START_MODULE(payload) {
  // Connect to the IPC
  OK(ipc_connect(ipc.pay.name))

  // Create listener for general requests
  OK(ipc_qrecv("*", general, NULL, IPC_QRECV_MSG))

  // Keep refreshing incoming messages
  for (;;) {
    OK(ipc_refresh())
  }
}

STOP_MODULE(payload) {
  // Disconnect from the IPC
  ipc_disconnect();
}

EXPORT_MODULE(payload);