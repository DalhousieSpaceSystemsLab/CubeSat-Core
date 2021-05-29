#include "mission_module.h"

// Define GPS coordinate range
float range[4] = {10.0f, 10.0f, 45.0f, 45.0f};

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

    // Read GPS coordinate with timeout
    char gps_coor[MAX_MSG_LEN];
    int bytes_read = 0;
    IF_TIMEOUT(bytes_read =
                   ipc_recv(ipc.gps.name, gps_coor, MAX_MSG_LEN, RECV_TIMEOUT),
               continue);
    OK(bytes_read);
    gps_coor[bytes_read] = '\0';

    // DEBUG
    if (strlen(gps_coor) > 0) printf("%s\n", gps_coor);

    // Parse coordinates
    char lat_str[32], long_str[32];
    float lattitude = 0, longitude = 0;
    for (int x = 0; x < strlen(gps_coor); x++) {
      if (gps_coor[x] == ',') {
        lat_str[x] = '\0';
        strcpy(long_str, &gps_coor[x + 1]);
        break;
      } else {
        lat_str[x] = gps_coor[x];
      }
    }
    lattitude = atof(lat_str);
    longitude = atof(long_str);

    // Check if in range
    if (range[0] <= lattitude && lattitude <= range[2] &&
        range[1] <= longitude && longitude <= range[3]) {
      modprintf("We are in range\n");

      // Send message to payload
      OK(ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic));
    }

    // Refresh background listeners
    OK(ipc_refresh());
  }
}

STOP_MODULE(mission) { ipc_disconnect(); }

EXPORT_MODULE(mission);