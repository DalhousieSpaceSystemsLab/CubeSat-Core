#include "mission_module.h"

CALLBACK(general) { modprintf("incoming message: %.*s\n", msg_len, msg); }

START_MODULE(mission) {
  OK(ipc_connect(ipc.core.msn.name))

  OK(ipc_qrecv("*", general, NULL, IPC_QRECV_MSG))

  // Send payload the command every 5 seconds
  modprintf("Sending message to payload module...");
  ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic);
  modprintf("Done!");
  for (;;) {
    OK(ipc_refresh())
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);