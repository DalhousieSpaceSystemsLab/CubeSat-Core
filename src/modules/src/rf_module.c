/**
 * @file rf_module.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Radio module for the satellite
 * @version 0.1
 * @date 2021-06-02
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "rf_module.h"

START_MODULE(rf) {
  OK(ipc_connect(ipc.trx.name));

  // Send command to command module every 60 seconds
  for (;;) {
    OK(ipc_send_cmd(ipc.core.cmd.name, "%s %s %f %f %f %f",
                    ipc.core.cmd.cmd.take_picture, "gps", 0.0f, 0.0f, 60.0f,
                    60.0f));
    OK(ipc_refresh());
    sleep(60);
  }
}

STOP_MODULE(rf) { OK(ipc_disconnect()); }

EXPORT_MODULE(rf);