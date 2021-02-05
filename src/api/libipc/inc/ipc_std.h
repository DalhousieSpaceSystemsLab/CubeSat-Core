/**
 * std.h
 * 
 * purpose: provide standard data structures and values for all ipc-related matters.
 * author:  alex amellal
 * 
 */


#ifndef CUBESAT_CORE_INCLUDE_IPC_STD_H
#define CUBESAT_CORE_INCLUDE_IPC_STD_H

/**
 * The IPC struct exists to provide standard values for 
 * subsystems and core software servers.
 */
struct ipc {
  // Payload
  struct pay {
    const char* name;
    struct cmd_pay {
      const char* take_pic;
    } cmd;
  } pay;

  // Comms
  struct trx {
    const char* name;
    struct cmd_trx {

    } cmd;
  } trx;

  // ACDS
  struct acs {
    const char* name;
    struct cmd_acs {

    } cmd;
  } acs;

  // ECS
  struct ecs {
    const char* name;
    struct cmd_ecs {

    } cmd;
  } ecs;
  // Core software
  struct core {
    // Missions
    struct msn {
      const char* name;
      struct cmd_msn {

      } cmd;
    } msn;
    // Commands
    struct cmd {
      const char* name;
      struct cmd_cmd {

      } cmd;
    } cmd;
    // Logging
    struct log {
      const char* name;
      struct cmd_log {

      } cmd;
    } log;
    // File system 
    struct fls {
      const char* name;
      struct cmd_fls {
        const char* save;
      } cmd;
    } fls;
  } core;
};

extern struct ipc ipc;

#endif // End of CUBESAT_CORE_INCLUDE_IPC_STD_H header guard
