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
    // Get GPS coordinate
    OK(ipc_send_cmd(ipc.gps.name, ipc.gps.cmd.get_cur_pos));

    // Read GPS coordinate
    char gps_coor[MAX_MSG_LEN];
    int bytes_read = ipc_recv(ipc.gps.name, gps_coor, MAX_MSG_LEN);
    OK(bytes_read);

    // DEBUG
    printf("%s\n", gps_coor);

    // ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic);
    OK(ipc_refresh())
    sleep(1);
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);