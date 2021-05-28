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

    .gps = {.name = "gps", .cmd = {.get_cur_pos = "0x67890"}},

    .core = {
        .fls = {.name = "fls",
                .cmd = {.fsend = "fsend", .fsend_list = "fsend_list"}},
        .msn = {.name = "msn"},
    }};