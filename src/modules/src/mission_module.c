#include "mission_module.h"

START_MODULE(mission) {
  OK(ipc_connect(ipc.core.msn.name));

  // Send payload the command every 5 seconds
  for (;;) {
    OK(ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic));
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);