/**
 * std.c
 *
 * purpose: provide standard data structures and values for all ipc-related
 * matters. author:  alex amellal
 *
 */

#include "ipc_std.h"

struct ipc ipc = {
    .pay = {.name = "pay", .cmd = {.take_pic = "0x12345"}},
    .trx = {.name = "trx"},
    .acs = {.name = "acs"},
    .ecs = {.name = "ecs"},
    .gps = {.name = "gps", .cmd = {.get_cur_pos = "0x67890"}},

    .core =
        {
            .fls = {.name = "fls"},
            .msn = {.name = "msn"},
            .cmd = {.name = "cmd", .cmd = {.take_picture = "0x3256489632"}},
            .log = {.name = "log"},
        },
};