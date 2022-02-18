/**
 * payload.c
 *
 * purpose: handles operations related to the payload susbystem
 * author:  alex amellal
 *
 */

#include "payload_module.h"

// change main name to pay_start
static int pay_start(int argc, char const *argv) {
  char input[50];
  // argv[0] - name of program
  // argv[1] - command typed into command line, ipc.pay.cmd.pic count as one, if
  // space between then 2
  strcpy(input, argv);
  printf("Camera function starting...\n");
  camera(input);

  return 0;
}

static int camera(char *command) {
  time_t begin = clock();  // is this needed?
  // char image_filename[100];
  // char order[50];
  // strcpy(order, ipc_qrecv());

  //**************************************************
  // Fill array with input command
  printf("camera() executed...\n");
  
  char *token = strtok(command, ".");  // parse input into separate values
  char ipc[3], subsystem[3], function[3], argument[20], value[10];

  int loop = 0;

  while (token != NULL) {
    if (loop == 0) {
      strcpy(ipc, token);  // ipc
    } else if (loop == 1) {
      strcpy(subsystem, token);  // pay
    } else if (loop == 2) {
      strcpy(function, token);  // cmd or dat
    } else if (loop == 3) {
      strcpy(argument, token);  // specific data to change, ex: burst_set
    } else if (loop == 4) {
      strcpy(value, token);  // in case there are different options for argument
    }
    token = strtok(NULL, ".");
    loop++;
  }

  //**************************************************
  // If tree
  if (strcmp(ipc, "ipc") == 0) {
    if (strcmp(subsystem, "pay") == 0) {
      // commands such as take picture
      if (strcmp(function, "cmd") == 0) {
        // Commence photo taking
        if (strcmp(argument, "take_pic") == 0) {
          // open_device();
          // init_device();
          printf("opened and init-ed camera\n");

          printf("Taking picture...\n");
          // start_capturing();
          // mainloop();
          // stop_capturing();
          printf("capture complete\n");

          // uninit_device();
          // close_device();
          fprintf(stderr, "\n");
          printf("closed and uninit-ed camera\n");
        }
      }
      if (strcmp(function, "dat") == 0) {
        if (strcmp(argument, "pixel_format") == 0) {
          // currently, on line 754 - capture.c
          // c_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264
          // says to replace ?
          // would put settings for both color and mono in here
        }
        // set number of photos to burst, ipc.pay.dat.burst_set.20 makes camera
        // take 20 pics
        if (strcmp(argument, "burst_set") == 0)  // set number of frames to take
        {
          // variable initialized in capture.c
          frame_count = atoi(value);
          printf("set frames capture to %s\n", value);
        }
        // set to single shot
        if (strcmp(argument, "single_set") ==
            0)  // set number of frames to take to 1
        {
          frame_count = 1;
          printf("set frames capture to 1\n");
        }

        // COLOR CAMERA

        // according to datasheet - res width 256 to 3072 px - res height 4 to
        // 2048 px set resolution for color camera w - width, h - height
        if (strcmp(argument, "c_res_w") == 0) {
          strcpy(c_res_wid, value);
          printf("c-res width set to %s\n", value);
        }
        if (strcmp(argument, "c_res_h") == 0) {
          strcpy(c_res_hgt, value);
          printf("c-res height set to %s\n", value);
        }
        // modify brightness
        if (strcmp(argument, "c_bri_set") == 0) {
          strcpy(c_bright, value);
          printf("c-brightness set to %s\n", value);
        }
        // modify gain
        if (strcmp(argument, "c_gain_set") == 0) {
          strcpy(c_gain, value);
          printf("c-gain set to %s\n", value);
        }

        // modify white balance
        // source -
        // https://www.kernel.org/doc/html/v4.8/media/uapi/v4l/extended-controls.html
        // value 0 to 9
        // 0 - V4L2_WHITE_BALANCE_AUTO
        // 1 - V4L2_WHITE_BALANCE_INCANDESCENT
        // 2 - V4L2_WHITE_BALANCE_FLUORESCENT
        // 3 - V4L2_WHITE_BALANCE_FLUORESCENT_H
        // 4 - V4L2_WHITE_BALANCE_HORIZON
        // 5 - V4L2_WHITE_BALANCE_DAYLIGHT
        // 6 - V4L2_WHITE_BALANCE_FLASH
        // 7 - V4L2_WHITE_BALANCE_CLOUDY
        // 8 - V4L2_WHITE_BALANCE_SHADE
        // 9 - V4L2_WHITE_BALANCE_MANUAL ... maybe not possible
        if (strcmp(argument, "c_wb_set") == 0) {
          if (strcmp(value, "0") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_AUTO;
            printf("c-white balance set to V4L2_WHITE_BALANCE_AUTO\n");
          }
          if (strcmp(value, "1") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_INCANDESCENT;
            printf("c-white balance set to V4L2_WHITE_BALANCE_INCANDESCENT\n");
          }
          if (strcmp(value, "2") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_FLUORESCENT;
            printf("c-white balance set to V4L2_WHITE_BALANCE_FLUORESCENT\n");
          }
          if (strcmp(value, "3") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_FLUORESCENT_H;
            printf("c-white balance set to V4L2_WHITE_BALANCE_FLUORESCENT_H\n");
          }
          if (strcmp(value, "4") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_HORIZON;
            printf("c-white balance set to V4L2_WHITE_BALANCE_HORIZON\n");
          }
          if (strcmp(value, "5") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_DAYLIGHT;
            printf("c-white balance set to V4L2_WHITE_BALANCE_DAYLIGHT\n");
          }
          if (strcmp(value, "6") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_FLASH;
            printf("c-white balance set to V4L2_WHITE_BALANCE_FLASH\n");
          }
          if (strcmp(value, "7") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_CLOUDY;
            printf("c-white balance set to V4L2_WHITE_BALANCE_CLOUDY\n");
          }
          if (strcmp(value, "8") == 0) {
            c_white_bal = V4L2_WHITE_BALANCE_SHADE;
            printf("c-white balance set to V4L2_WHITE_BALANCE_SHADE\n");
          }
        }
        // modify scene
        // source -
        // https://www.kernel.org/doc/html/v4.8/media/uapi/v4l/extended-controls.html
        // value 0 to 8
        // 0 - V4L2_SCENE_MODE_NONE
        // 1 - V4L2_SCENE_MODE_BACKLIGHT
        // 2 - V4L2_SCENE_MODE_DAWN_DUSK
        // 3 - V4L2_SCENE_MODE_FALL_COLORS
        // 4 - V4L2_SCENE_MODE_FIREWORKS
        // 5 - V4L2_SCENE_MODE_LANDSCAPE
        // 6 - V4L2_SCENE_MODE_NIGHT
        // 7 - V4L2_SCENE_MODE_SPORTS
        // 8 - V4L2_SCENE_MODE_SUNSET
        if (strcmp(argument, "c_sc") == 0) {
          if (strcmp(value, "0") == 0) {
            c_scene = V4L2_SCENE_MODE_NONE;
            printf("c-scene set to V4L2_SCENE_MODE_NONE\n");
          }
          if (strcmp(value, "1") == 0) {
            c_scene = V4L2_SCENE_MODE_BACKLIGHT;
            printf("c-scene set to V4L2_SCENE_MODE_BACKLIGHT\n");
          }
          if (strcmp(value, "2") == 0) {
            c_scene = V4L2_SCENE_MODE_DAWN_DUSK;
            printf("c-scene set to V4L2_SCENE_MODE_DAWN_DUSK\n");
          }
          if (strcmp(value, "3") == 0) {
            c_scene = V4L2_SCENE_MODE_FALL_COLORS;
            printf("c-scene set to V4L2_SCENE_MODE_FALL_COLORS\n");
          }
          if (strcmp(value, "4") == 0) {
            c_scene = V4L2_SCENE_MODE_FIREWORKS;
            printf("c-scene set to V4L2_SCENE_MODE_FIREWORKS\n");
          }
          if (strcmp(value, "5") == 0) {
            c_scene = V4L2_SCENE_MODE_LANDSCAPE;
            printf("c-scene set to V4L2_SCENE_MODE_LANDSCAPE\n");
          }
          if (strcmp(value, "6") == 0) {
            c_scene = V4L2_SCENE_MODE_NIGHT;
            printf("c-scene set to V4L2_SCENE_MODE_NIGHT\n");
          }
          if (strcmp(value, "7") == 0) {
            c_scene = V4L2_SCENE_MODE_SPORTS;
            printf("c-scene set to V4L2_SCENE_MODE_SPORTS\n");
          }
          if (strcmp(value, "8") == 0) {
            c_scene = V4L2_SCENE_MODE_SUNSET;
            printf("c-scene set to V4L2_SCENE_MODE_SUNSET\n");
          }
        }

        // MONO CAMERA

        // set resolution for mono camera
        if (strcmp(argument, "m_res_w") == 0) {
          strcpy(m_res_wid, value);
          printf("m-res width set to %s\n", value);
        }
        if (strcmp(argument, "m_res_h") == 0) {
          strcpy(m_res_hgt, value);
          printf("m-res height set to %s\n", value);
        }
        // modify brightness
        if (strcmp(argument, "m_bri_set") == 0) {
          strcpy(m_bright, value);
          printf("m-brightness set to %s\n", value);
        }
        // modify gain
        if (strcmp(argument, "m_gain_set") == 0) {
          strcpy(m_gain, value);
          printf("m-gain set to %s\n", value);
        }
        // modify white balance
        if (strcmp(argument, "m_wb_set") == 0) {
          // TODO: white balance does not exist for mono cameras...?
          // figure out what values can be assigned
          printf("m-auto white balance set to %s\n", value);
        }
        // modify scene
        if (strcmp(argument, "m_sc") == 0) {
          // TODO: scenes do not exist for mono camera...?
          // change contrast
          printf("m-scene set to %s\n", value);
        }
      }
    }
  }

  // char command[100] = "cd /home/pi/Pictures/payloadcapture/;split -b20000 ";
  // system(strcat(command,&filename));
  // printf("filename: %s\n", image_filename);
  printf("Time elapsed is %f seconds",
         ((clock() - begin) / ((float)CLOCKS_PER_SEC)));

  return 0;
}

CALLBACK(payload_general) {
  // process msg/command

  // checks that first 8 incoming characters match to payload commands
  if (strncmp(msg, "ipc.pay.", 8) == 0) 
  {
    printf("Payload Subsystem starting...\n");

    // char **argv = NULL;
    pay_start(0, msg);


    modprintf("done!\n");
  }
  else 
  {
    modprintf("misc message incoming: %.*s\n", msg_len, msg);
  }

  // done
  STOP_CALLBACK;
}

START_MODULE(payload) {
  // Connect to the IPC
  OK(ipc_connect(ipc.pay.name))

  // Create listener for general requests
  OK(ipc_qrecv("*", payload_general, NULL, IPC_QRECV_MSG))

  // Keep refreshing incoming messages
  for (;;) {
    OK(ipc_refresh())
  }
}

STOP_MODULE(payload) {
  // Disconnect from the IPC
  ipc_disconnect();
  modprintf("disconnecting payload\n");
}

EXPORT_MODULE(payload);