#include "gps_module.h"

// Constants
#define LATITUDE_FILE_SIZE 49077
#define LONGITUDE_FILE_SIZE 49729

// Glocal variables
static char *latitude_raw = NULL;
static char *longitude_raw = NULL;
static unsigned int coor_served = 0;

static int load_gps_data() {
  // Check if heap initialized
  if (latitude_raw != NULL) {
    FILE *file_lat = fopen("latitude.txt", "r");  // Open file
    if (file_lat == NULL) {                       // File failed to open
      moderr("Failed to load latitude file\n");
      return -1;
    }

    fgets(latitude_raw, LATITUDE_FILE_SIZE,
          file_lat);  // Read entire file contents

    fclose(file_lat);  // Close file
  } else {
    moderr("latitude_raw heap not initialized\n");
    return -1;
  }

  // Check if heap initialized
  if (longitude_raw != NULL) {
    FILE *file_long = fopen("longitude.txt", "r");
    if (file_long == NULL) {
      moderr("Failed to load longitude file\n");
      return -1;
    }

    fgets(longitude_raw, LONGITUDE_FILE_SIZE, file_long);

    fclose(file_long);
  } else {
    moderr("longitude_raw heap not initialized\n");
  }

  return 0;
}

static int load_gps_coor(unsigned int index, char buffer[64]) {
  // Check if data initialized
  if (latitude_raw == NULL || longitude_raw == NULL) {
    moderr("load_gps_coor failed : gps data not initialized\n");
    return -1;
  }

  // Load nth latitude
  unsigned int n = 0;
  char lat[32];
  for (unsigned int x = 0; x < LATITUDE_FILE_SIZE; x++) {
    // Check if index matches nth coordinate
    if (n == index) {
      // Load lattitude
      for (int y = 0; y < 32; y++) {
        if (latitude_raw[y] == '\n') {
          lat[y] = '\0';
          break;
        }
        lat[y] = latitude_raw[x + y];
      }
      break;
    }

    // Parse through latitude data
    if (latitude_raw[x] == '\n') {
      n++;
    }
  }

  char lon[32];
  for (unsigned int x = 0; x < LONGITUDE_FILE_SIZE; x++) {
    // Check if index matches nth coordinate
    if (n == index) {
      // Load lattitude
      for (int y = 0; y < 32; y++) {
        if (longitude_raw[y] == '\n') {
          lon[y] = '\0';
          break;
        }
        lon[y] = longitude_raw[x + y];
      }
      break;
    }

    // Parse through latitude data
    if (longitude_raw[x] == '\n') {
      n++;
    }
  }

  // DEBUG
  modprintf("[index = %d] latitude = %s, longitude = %s\n", index, lat, lon);

  // done
  return 0;
}

CALLBACK(gps_general) {
  // Retrieve

  STOP_CALLBACK;
}

START_MODULE(gps) {
  // Connect to the IPC
  OK(ipc_connect(ipc.gps.name));

  // Load latitude and longitude data
  latitude_raw = (char *)malloc(50000);   // Allocate 50K of heap for latitude
  longitude_raw = (char *)malloc(50000);  // Allocate 50K of heap for longitude

  // Load GPS data into heap
  OK(load_gps_data());

  // Set number of coordinates served to 0
  coor_served = 0;

  for (int x = 0; x < 10; x++) {
    char coor[64];
    load_gps_coor(x, coor);
  }
}

STOP_MODULE(gps) {
  // Free heap
  if (latitude_raw != NULL) free(latitude_raw);
  if (longitude_raw != NULL) free(longitude_raw);

  // Disconnect from the IPC
  OK(ipc_disconnect());
}

EXPORT_MODULE(gps);