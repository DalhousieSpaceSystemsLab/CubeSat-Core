#include "mission_module.h"

CALLBACK(mission_general) {
  modprintf("incoming message: %.*s\n", msg_len, msg);

  // done
  STOP_CALLBACK;
}

START_MODULE(mission) {
  OK(ipc_connect(ipc.core.msn.name))

  OK(ipc_qrecv("*", mission_general, NULL, IPC_QRECV_MSG))

  // Send payload the command every 5 seconds
  for (;;) {
    ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic);
    OK(ipc_refresh())
    // sleep(3);
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);