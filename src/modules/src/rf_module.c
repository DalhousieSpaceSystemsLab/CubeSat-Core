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

  // Send command to command module every 10 seconds
  for (;;) {
    OK(ipc_send_cmd(ipc.core.cmd.name, "%s %s %f %f",
                    ipc.core.cmd.cmd.take_picture, "gps", 15.345845f,
                    20.458347f));

    sleep(10);
  }
}

STOP_MODULE(rf) { OK(ipc_disconnect()); }

EXPORT_MODULE(rf);