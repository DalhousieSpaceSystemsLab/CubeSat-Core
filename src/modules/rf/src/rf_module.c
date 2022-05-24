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

static int process_req(char req[2]);
static int return_basic_telemetry();
static int listen_file();
static int return_file();
static int create_ls_index();
static int return_ls();
static int take_picture();
static int encode_file();
static int decode_file();
static int shutdown_cmd();
static int reboot();
static int rm_file();
static int mv_file();
static int forward_command();

START_MODULE(rf) {
  OK(ipc_connect(ipc.rf.name));

  // Init antenna
  OK(antenna_init(PATH_TO_ANTENNA_DEV))
  OK(antenna_encode_init());

  // Enter autonomous mode
  for (;;) {
    // Wait for request
    char req[2];
    ON_FAIL(antenna_read(req, 2, READ_MODE_UNTIL), continue);

    // Identify request
    ON_FAIL(process_req(req), continue);
  }
}

static int process_req(char req[2]) {
  if (strncmp(req, REQ_BASIC_TELEMETRY, 2) == 0) {
    OK(return_basic_telemetry());
  } else if (strncmp(req, REQ_LARGE_TELEMETRY, 2) == 0) {
  } else if (strncmp(req, REQ_DELET_TELEMETRY, 2) == 0) {
  } else if (strncmp(req, REQ_REBOOT_OBC, 2) == 0) {
  } else if (strncmp(req, REQ_RESET_COMMS, 2) == 0) {
  } else if (strncmp(req, REQ_ENABLE_RAVEN, 2) == 0) {
  } else if (strncmp(req, REQ_FWD_COMMAND, 2) == 0) {
    OK(forward_command());
  } else if (strncmp(req, REQ_LISTEN_FILE, 2) == 0) {
    OK(listen_file());
  } else if (strncmp(req, REQ_GET_FILE, 2) == 0) {
    OK(return_file());
  } else if (strncmp(req, REQ_GET_LS, 2) == 0) {
    OK(return_ls());
  } else if (strncmp(req, REQ_TAKE_PICTURE, 2) == 0) {
    OK(take_picture());
  } else if (strncmp(req, REQ_ENCODE_FILE, 2) == 0) {
    OK(encode_file());
  } else if (strncmp(req, REQ_DECODE_FILE, 2) == 0) {
    OK(decode_file());
  } else if (strncmp(req, REQ_SHUTDOWN, 2) == 0) {
    OK(shutdown_cmd());
  } else if (strncmp(req, REQ_REBOOT, 2) == 0) {
    OK(reboot());
  } else if (strncmp(req, REQ_REMOVE, 2) == 0) {
    OK(rm_file());
  } else if (strncmp(req, REQ_MOVE, 2) == 0) {
    OK(mv_file());
  } else {
    moderr("[:/] Could not process request [%c%c]\n", req[0], req[1]);
  }

  // done
  return 0;
}

static int return_basic_telemetry() {
  // Send telemetry file
  modprintf("[i] Basic telemetry request received!\n");
  if (antenna_fwrite(FILE_BASIC_TELEMETRY) == -1) {
    modprintf("[!] Failed to send telemetry file to fulfill request\n");
    return -1;
  }

  // done
  return 0;
}

static int listen_file() {
  // Get filename
  char filename[MAX_FILENAME_LEN];
  // OK(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL));
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Listen for incoming file
  modprintf("[i] File listen request received!\n");
  if (antenna_fread(filename) == -1) {
    moderr("[!] Failed to read incoming file over the antenna\n");
    return -1;
  }
}

static int return_file() {
  // Prepare outgoing file
  int status = 0;
  modprintf("[i] File request received!\n");

  // Get filename
  char filename[MAX_FILENAME_LEN];
  // OK(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL));
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Check if file exists
  FILE *fp = fopen(filename, "r");
  if (fp) {
    // File found, send back
    ON_FAIL_LOG(antenna_fwrite(filename), status = -1; goto cleanup);
  } else {
    // File not found, return failure
    OK(antenna_write(FILE_NOT_FOUND, strlen(FILE_NOT_FOUND)));
    status = -1;
    goto end;
  }

cleanup:
  fclose(fp);
end:
  // done
  return status;
}

static int create_ls_index() {
  int status = 0;

  // Open ls index file
  FILE *findex = fopen(FILE_LS_INDEX, "w");
  if (!findex) {
    moderr("Failed to open LS index file\n");
    return -1;
  }

  // Get directory name
  char filename[MAX_FILENAME_LEN];
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Open current directory
  // DIR *d;
  // struct dirent *dir;
  // d = opendir(filename);
  // if (!d) {
  //   moderr("Failed to open current directory\n");
  //   status = -1;
  //   goto cleanup;
  // }

  // // Directory open, reading names
  // while ((dir = readdir(d)) != NULL) {
  //   fprintf(findex, "%s\n", dir->d_name);
  // }
  // closedir(d);

  char command[MAX_FILENAME_LEN * 2];
  sprintf(command, "ls -la %s > %s", filename, FILE_LS_INDEX);
  system(command);

cleanup:
  fclose(findex);

  // done
  return status;
}

static int return_ls() {
  // Create ls index
  OK(create_ls_index());

  // Send file
  OK(antenna_fwrite(FILE_LS_INDEX));

  // done
  return 0;
}

static int take_picture() {
  // Send req to payload
  OK(ipc_send_cmd(ipc.pay.name, ipc.pay.cmd.take_pic));

  // done
  return 0;
}

static int encode_file() {
  // Get filename
  char filename[MAX_FILENAME_LEN];
  // OK(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL));
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Encode file
  OK(antenna_encode_file(filename));

  // done
  return 0;
}

static int decode_file() {
  // Get filename
  char filename[MAX_FILENAME_LEN];
  // OK(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL));
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Decode file
  OK(antenna_decode_file(filename));

  // done
  return 0;
}

static int shutdown_cmd() {
  system("shutdown now");
  return 0;
}

static int reboot() {
  system("reboot");
  return 0;
}

static int rm_file() {
  // Get filename
  char filename[MAX_FILENAME_LEN];
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Delete file
  char command[MAX_FILENAME_LEN + 7];
  sprintf(command, "rm -rf %s", filename);
  system(command);

  // done
  return 0;
}

static int mv_file() {
  // Get filenames
  char filename[MAX_FILENAME_LEN];
  char filename_dest[MAX_FILENAME_LEN];
  IF_TIMEOUT_LOG(antenna_read(filename, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);
  IF_TIMEOUT_LOG(antenna_read(filename_dest, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Move file
  char command[2 * MAX_FILENAME_LEN + 3];
  sprintf(command, "mv %s %s", filename, filename_dest);
  system(command);

  // done
  return 0;
}

static int forward_command() {
  // Get command
  char command[MAX_FILENAME_LEN];
  IF_TIMEOUT_LOG(antenna_read(command, MAX_FILENAME_LEN, READ_MODE_UNTIL),
                 return -1);

  // Pass command to system
  system(command);

  // done
  return 0;
}

STOP_MODULE(rf) { OK(ipc_disconnect()); }

EXPORT_MODULE(rf);