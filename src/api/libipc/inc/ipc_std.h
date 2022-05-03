/**
 * std.h
 *
 * purpose: provide standard data structures and values for all ipc-related
 * matters. author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_STD_H
#define CUBESAT_CORE_INCLUDE_IPC_STD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The IPC struct exists to provide standard values for
 * subsystems and core software servers.
 */
struct ipc {
  // Global commands
  struct global {
    const char* take_picture;
  } global;

  // Payload
  struct pay {
    const char* name;
    struct cmd_pay {
      const char* take_pic;
    } cmd;
  } pay;

  // Comms
  struct rf {
    const char* name;
    struct cmd_rf {
    } cmd;
  } rf;

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

  // GPS
  struct gps {
    const char* name;
    struct cmd_gps {
      const char* get_cur_pos;
    } cmd;
  } gps;

  // Core software
  struct core {
    // Missions
    struct msn {
      const char* name;
      struct cmd_msn {
        const char* qmsn;
      } cmd;
    } msn;
    // Commands
    struct cmd {
      const char* name;
      struct cmd_cmd {
        const char* take_picture;
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
      } cmd;
    } fls;
  } core;
};

extern struct ipc ipc;

#ifdef __cplusplus
}
#endif

#endif  // End of CUBESAT_CORE_INCLUDE_IPC_STD_H header guard
