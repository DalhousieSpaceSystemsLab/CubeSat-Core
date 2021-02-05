/**
 * std.c
 * 
 * purpose: provide standard data structures and values for all ipc-related matters.
 * author:  alex amellal
 * 
 */

#include "ipc_std.h"

struct ipc ipc = {
  .pay = {
    .name = "pay",
    .cmd = {
      .take_pic = "0x12345"
    }
  },

  .core = {
    .fls = {
      .name = "fls",
      .cmd = {
        .save = "save 0x12345"
      }
    }
  }
};