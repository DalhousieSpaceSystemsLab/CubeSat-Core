/**
 * payload.c
 *
 * purpose: handles operations related to the payload susbystem
 * author:  alex amellal
 *
 */

#include "payload_module.h"
#include "client_api.h"

static void process_general_msg(char* msg, size_t msg_len, void* data) {
  // Take pic
  if (strncmp(msg, ipc.pay.cmd.take_pic, msg_len) == 0) {
    printf("[pay] taking picture...\n");
    printf("[pay] done!\n");
  } else {
    printf("[pay] misc message incoming: %.*s\n", msg_len, msg);
  }
}

START_MODULE(payload) {
  // Connect to the IPC
  if (ipc_connect(ipc.pay.name) != 0) {
    fprintf(stderr, "[pay] failed to connect to the IPC\n");
    return 0;
  }

  // Create listener for general requests
  if (ipc_qrecv("*", process_general_msg, NULL, IPC_QRECV_MSG) < 0) {
    fprintf(stderr, "[pay] ipc_qrecv(\"*\") failed\n");
    return 0;
  }

  // Keep refreshing incoming messages
  for (;;) {
    if (ipc_refresh() < 0) {
      fprintf(stderr, "[pay] ipc_refresh() failed\n");
      return 0;
    }
  }
}

STOP_MODULE(payload) {
  // Disconnect from the IPC
  ipc_disconnect();
}

EXPORT_MODULE(payload)