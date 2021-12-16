/**
 * std.c
 *
 * purpose: provide standard data structures and values for all ipc-related
 * matters. author:  alex amellal
 *
 */

#include "ipc_std.h"

struct ipc ipc = {
    .pay = {.name = "pay", .cmd = {.take_pic = "ipc://pay.take_pic"}},
    .trx = {.name = "trx"},
    .acs = {.name = "acs"},
    .ecs = {.name = "ecs"},
    .gps = {.name = "gps", .cmd = {.get_cur_pos = "ipc://gps.get_cur_pos"}},

    .core =
        {
            .fls = {.name = "fls"},
            .msn = {.name = "msn",
                    .cmd = {.qmsn = "ipc://core.cmd.queue_mission"}},
            .cmd = {.name = "cmd",
                    .cmd = {.take_picture = "ipc://core.cmd.take_picture"}},
            .log = {.name = "log"},
        },
};