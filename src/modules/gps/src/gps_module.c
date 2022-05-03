#include "gps_module.h"

char gps_coord_output[50] = {'t', 'e', 's', 't'};

void* gen_gps_coord(void* rec_gps) {
  int i = 0;
  // long_record* long_rec_data_arr_temp =
  // (long_record*)malloc(sizeof(long_record)*2667);
  struct gps_coord_rec* struct_ptr = (struct gps_coord_rec*)rec_gps;
  while (1) {
    // Delay for approx 1000ms. 32767 is approx 1s.
    // Should use this same delay for mission server, end of the day the timming
    // should be close enough for a demo.
    struct timespec delay = {
        .tv_sec = 0,
        .tv_nsec = 500000000l,
    };
    nanosleep(&delay, NULL);
    // Assemble the GPS coordinates into one string with a comma delimieters.
    strcpy(gps_coord_output, (struct_ptr + i)->gps_coord_long);
    strcat(gps_coord_output, ",");
    strcat(gps_coord_output, (struct_ptr + i)->gps_coord_lat);
    strcat(gps_coord_output, ",");
    // printf("[gps] Current location: %s\n",gps_coord_output);//Output current
    // GPS coordinates to the console.
    i++;
    (i == 2667 ? i = 0 : 0);
  }
}

CALLBACK(gps_general) {
  /* Assume message is coming from the mission server */

  // Check if requesting GPS coordinates
  if (strncmp(msg, ipc.gps.cmd.get_cur_pos, strlen(msg)) == 0) {
    // Return GPS coordinates
    OK(ipc_send(ipc.core.msn.name, gps_coord_output, strlen(gps_coord_output)));
  }
  STOP_CALLBACK;
}

START_MODULE(gps) {
  // Connect to the IPC
  OK(ipc_connect(ipc.gps.name));

  // Setup background listener
  modprintf("Setting up background listener\n");
  OK(ipc_create_listener("*", gps_general, NULL));
  // Retreive the GPS coordinates from .txt files and store them in arrays
  // before the thread begins.
  int i = 0;
  // Record and file pointers for latitude array.
  gps_coord_rec* gps_coord_rec_arr =
      (gps_coord_rec*)malloc(sizeof(gps_coord_rec) * 2667);
  FILE* gps_coord_temp = NULL;

  // DEBUG
  modprintf(
      "Retrieving coordinates from .txt file "
      "(gps_coord_retreive())...\n");

  // Retrive lattitude coordinates from .txt file and store in  lat_rec_data_arr
  // array.
  gps_coord_retreive(gps_coord_rec_arr, gps_coord_temp);
  // for (int j=0;j<2667;j++){
  // printf("%s",(lat_rec_data_arr + j)->gps_coord_lat);
  //}

  modprintf("Done!\n");
  modprintf("Creating thread for coordinate generation\n");

  pthread_t thread_gen_gps_coord;
  pthread_create(&thread_gen_gps_coord, NULL, gen_gps_coord,
                 &gps_coord_rec_arr[0]);

  modprintf("Done!\n");

  modprintf("Starting refresh loop...\n");
  for (;;) {
    OK(ipc_refresh());
  }

  return 0;
}

STOP_MODULE(gps) {
  // Disconnect from the IPC
  OK(ipc_disconnect());

  return 0;
}

void gps_coord_retreive(gps_coord_rec* gps_record_address, FILE* gps_file) {
  int i = 0;
  char lat_data_arr_temp[2667], *lat_token;
  // Open latitude.txt file: source file (read only), and new destination file
  // named "numfile.txt" (write only).
  if ((gps_file = fopen("latitude.txt", "r")) == NULL) {
    printf("[gps] ERROR - latitude.txt failed to open properly!!!\n");
  }
  // Parse the source file one line at a time for numbers, and store them in
  // numfile.txt.
  while (fgets(lat_data_arr_temp, 2667, gps_file) !=
         NULL)  // Stores one row of text from the source file up to the first
                // "\n".
  {
    lat_token = strtok(lat_data_arr_temp, ",");
    strcpy((gps_record_address + i)->gps_coord_lat, lat_token);
    i++;
  }
  i = 0;
  char long_data_arr_temp[2667], *long_token;
  // Open longitude.txt file: source file (read only), and new destination file
  // named "numfile.txt" (write only).
  if ((gps_file = fopen("longitude.txt", "r")) == NULL) {
    printf("[gps] ERROR - longitude.txt failed to open properly!!!\n");
  }
  // Parse the source file one line at a time for numbers, and store them in
  // numfile.txt.
  while (fgets(long_data_arr_temp, 2667, gps_file) !=
         NULL)  // Stores one row of text from the source file up to the first
                // "\n".
  {
    long_token = strtok(long_data_arr_temp, ",");
    strcpy((gps_record_address + i)->gps_coord_long, long_token);
    i++;
  }
}

EXPORT_MODULE(gps);