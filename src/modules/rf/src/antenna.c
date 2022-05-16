#include "antenna.h"

// Glocal variables
static int uartfd = -1;

/**
 * @brief Initializes the UART port for the antenna.
 *
 * @param path Sets the device path to configure.
 * @return 0 on success, 1 on error.
 */
int antenna_init(const char *path) {
  if ((uartfd = open(path, O_RDWR | O_NOCTTY | O_SYNC)) < 0) {
    printf("[!] Failed to open I/O device at %s\n", path);
    return -1;
  }

  struct termios tty;

  if (tcgetattr(uartfd, &tty) < 0) {
    printf("Error from tcgetattr: %s\n", strerror(errno));
    return -1;
  }

  cfsetospeed(&tty, (speed_t)UART_SPEED);
  cfsetispeed(&tty, (speed_t)UART_SPEED);

  tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;      /* 8-bit characters */
  tty.c_cflag &= ~PARENB;  /* no parity bit */
  tty.c_cflag &= ~CSTOPB;  /* only need 1 stop bit */
  tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

  /* setup for non-canonical mode */
  tty.c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tty.c_oflag &= ~OPOST;

  /* fetch bytes as they become available */
  tty.c_cc[VMIN] = TERMIOS_VMIN;
  tty.c_cc[VTIME] = TERMIOS_VTIME;

  if (tcsetattr(uartfd, TCSANOW, &tty) != 0) {
    printf("Error from tcsetattr: %s\n", strerror(errno));
    return -1;
  }
}

// int antenna_init(const char *path) {
//   if ((uartfd = open(path, O_RDWR | O_NOCTTY | O_SYNC)) < 0) {
//     printf("[!] Failed to open I/O device at %s\n", path);
//     return -1;
//   }

//   struct termios oldtio, newtio;

//   if (tcgetattr(uartfd, &oldtio) < 0) { /* save current port settings */
//     printf("Error from tcgetattr: %s\n", strerror(errno));
//     return -1;
//   }

//   bzero(&newtio, sizeof(newtio));
//         newtio.c_cflag = UART_SPEED | CRTSCTS | CS8 | CLOCAL | CREAD;
//         newtio.c_iflag = IGNPAR;
//         newtio.c_oflag = 0;

//   /* set input mode (non-canonical, no echo,...) */
//   newtio.c_lflag = 0;

//   newtio.c_cc[VTIME]    = 5;   /* inter-character timer unused */
//   newtio.c_cc[VMIN]     = 1;   /* block read until 1 byte is read */

//   tcflush(uartfd, TCIFLUSH);
//   tcsetattr(uartfd,TCSANOW,&newtio);
// }

/**
 * @brief Identical to antenna_write, but allows a custom file descriptor to be
 * specified.
 *
 * @param fd File descriptor to use.
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_fd(int fd, const char *data, size_t data_len) {
  // Prepare packet
  struct antenna_packet p;
  if (antenna_packet_new(&p) < 0) {
    printf("[!] Failed to create new packet\n");
    return -1;
  }

  // DEBUG
  // printf("[DEBUG] Waiting for fd to be ready...\n");

  // DEBUG
  // printf("[DEBUG] Ready to write!\n");

  size_t total_bytes_written = 0;
  char buffer[WRITE_BUFFER_SIZE];
  while (total_bytes_written < data_len) {
    int bytes_remaining = data_len - total_bytes_written;
    int bytes_to_write = (WRITE_BUFFER_SIZE > bytes_remaining)
                             ? bytes_remaining
                             : WRITE_BUFFER_SIZE;
    memcpy(buffer, &data[total_bytes_written], bytes_to_write);
    if (write(fd, buffer, bytes_to_write) < bytes_to_write) {
      printf("[!] Failed to send data of %d bytes in length.\n", data_len);
      return -1;
    }
    total_bytes_written += bytes_to_write;
    usleep(WRITE_DELAY);
  }

  // Write bytes to antenna
  // if (write(fd, data, data_len) < data_len) {
  //   printf("[!] Failed to send data of %d bytes in length.\n", data_len);
  //   return -1;
  // }

  // done
  return 0;
}

/**
 * @brief Writes bytes to the antenna.
 *
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 *
 * @return 0 on success, -1 on error
 */
int antenna_write(const char *data, size_t data_len) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot write to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_write_fd(uartfd, data, data_len);
}

/**
 * @brief Writes bytes to the antenna with Reed-solomon FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use.
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_rs_fd(int fd, const char *data, size_t data_len) {
  // Return status
  int status = 0;

  // Encode data
  int bytes_encoded = 0;
  correct_reed_solomon *encoder = correct_reed_solomon_create(
      correct_rs_primitive_polynomial_8_4_3_2_0, 1, 1, RS_NUM_ROOTS);
  if (encoder == NULL) {
    printf("[!] Failed to create RS encoder\n");
    status = -1;
    goto cleanup;
  }
  while (bytes_encoded < data_len) {
    // Encode block of data
    int bytes_remaining = (data_len - bytes_encoded);
    int bytes_to_encode =
        (bytes_remaining > RS_DATA_LEN) ? RS_DATA_LEN : bytes_remaining;
    char data_encoded[255];
    int data_encoded_len = correct_reed_solomon_encode(
        encoder, &data[bytes_encoded], bytes_to_encode, data_encoded);
    if (data_encoded_len < 0) {
      printf("[!] Failed to encode data\n");
      status = -1;
      goto cleanup;
    }

    // Send block
    if (antenna_write_fd(fd, data_encoded, data_encoded_len) < 0) {
      printf("[!] Failed to send block of encoded data\n");
      status = -1;
      goto cleanup;
    }

    // Update counters
    bytes_encoded += bytes_to_encode;
  }

cleanup:
  // Free encoder
  correct_reed_solomon_destroy(encoder);

  // done
  return status;
}

/**
 * @brief Writes bytes to the antenna with Reed-solomon FEC
 *
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_rs(const char *data, size_t data_len) {
  return antenna_write_rs_fd(uartfd, data, data_len);
}

/**
 * @brief Reads bytes from the antenna but allows a custom file descriptor to be
 * specified. Note that there are 2 ways to read: (1) Read UP TO read_len bytes,
 * (2) Block until read_len bytes have been read.
 *
 * @param fd File descriptor to use.
 * @param buffer Output buffer array for incoming bytes.
 * @param read_len Read UP TO or block UNTIL this many bytes read.
 * @param read_mode Set to READ_MODE_UPTO or READ_MODE_UNTIL
 * @return number of bytes read or < 0 for error or TIMEOUT_OCCURED for timeout.
 */
int antenna_read_fd(int fd, char *buffer, size_t read_len, int read_mode) {
  TIMEOUT_START();

  // Create placeholders for reading
  size_t bytes_read = 0;

  // Check read mode
  if (read_mode == READ_MODE_UPTO) {
    if ((bytes_read = read(fd, buffer, read_len)) < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        printf("[!] Would have blocked, all good though.\n");
        return 0;
      }
      printf("[!] Failed to read from uartfd\n");
      return -1;
    }
  } else if (read_mode == READ_MODE_UNTIL) {
    while (bytes_read < read_len) {
      // Read bytes
      int new_bytes_read = -1;
      if ((new_bytes_read =
               read(fd, &buffer[bytes_read], read_len - bytes_read)) < 0) {
        printf("[!] Failed to read from fd\n");
        return -1;
      }

      // Update bytes read so far
      bytes_read += new_bytes_read;
      TIMEOUT_UPDATE();
      TIMEOUT_IF(READ_OP_TIMEOUT, break);
    }
  } else {
    printf("[!] Invalid read mode. Try READ_MODE_UPTO or READ_MODE_UNTIL\n");
    return -1;
  }

  // done
  RETURN_IF_TIMEOUT();
  return bytes_read;
}

/**
 * @brief Reads bytes from the antenna.
 * Note that there are 2 ways to read:
 * (1) Read UP TO read_len bytes,
 * (2) Block until read_len bytes have been read.
 *
 * @param buffer Output buffer array for incoming bytes.
 * @param read_len Read UP TO or block UNTIL this many bytes read.
 * @param read_mode Set to READ_MODE_UPTO or READ_MODE_UNTIL
 * @return number of bytes read or < 0 for error or TIMEOUT_OCCURED for timeout.
 */
int antenna_read(char *buffer, size_t read_len, int read_mode) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_read_fd(uartfd, buffer, read_len, read_mode);
}

/**
 * @brief Reads Reed-solomon encoded bytes from the antenna but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use.
 * @param buffer Output buffer array for incoming bytes.
 * @param read_len Read UP TO or block UNTIL this many bytes read.
 * @param read_mode Set to READ_MODE_UPTO or READ_MODE_UNTIL
 * @return number of bytes read or < 0 for error.
 */
int antenna_read_rs_fd(int fd, char *buffer, size_t read_len, int read_mode) {
  // Return status
  int status = 0;

  // Create encoder
  correct_reed_solomon *encoder = correct_reed_solomon_create(
      correct_rs_primitive_polynomial_8_4_3_2_0, 1, 1, RS_NUM_ROOTS);
  if (encoder == NULL) {
    printf("[!] Failed to create RS encoder\n");
    status = -1;
    goto cleanup;
  }

  // Create placeholders
  int bytes_decoded = 0;
  char data_in[RS_BLOCK_LEN];

  // Parse incoming blocks until length satisfied
  do {
    // Clear incoming buffer
    memset(data_in, 0, RS_BLOCK_LEN);

    // Read block
    int bytes_read = -1;
    if ((bytes_read = antenna_read_fd(fd, data_in, RS_BLOCK_LEN, read_mode)) <
        0) {
      printf("[!] Failed to read encoded block from antenna\n");
      status = -1;
      goto cleanup;
    }

    // Placeholder for decoded bytes
    char decoded[RS_DATA_LEN];

    // Decode it
    int new_bytes_decoded = -1;
    if ((new_bytes_decoded = correct_reed_solomon_decode(
             encoder, data_in, bytes_read, decoded)) < 0) {
      printf("[!] Failed to decode incoming block. SKIPPING.\n");
      // status = -1;
      // goto cleanup;
      continue;
    }

    // Copy decoded bytes into buffer
    int bytes_to_copy = (read_len - bytes_decoded) > new_bytes_decoded
                            ? new_bytes_decoded
                            : (read_len - bytes_decoded);
    memcpy(&buffer[bytes_decoded], decoded, bytes_to_copy);

    // Update counters
    bytes_decoded += bytes_to_copy;
  } while (bytes_decoded < read_len && read_mode == READ_MODE_UNTIL);

cleanup:
  // Free encoder
  correct_reed_solomon_destroy(encoder);

  // done
  if (status)
    return status;
  else
    return bytes_decoded;
}

/**
 * @brief Reads Reed-solomon encoded bytes from the antenna.
 *
 * @param buffer Output buffer array for incoming bytes.
 * @param read_len Read UP TO or block UNTIL this many bytes read.
 * @param read_mode Set to READ_MODE_UPTO or READ_MODE_UNTIL
 * @return number of bytes read or < 0 for error.
 */
int antenna_read_rs(char *buffer, size_t read_len, int read_mode) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_read_rs_fd(uartfd, buffer, read_len, read_mode);
}

static inline void display_progress(size_t n, size_t total, int width,
                                    char *what) {
  int progress = width * n / total;
  printf("\r%s progress (%lu/%lu): [", what, n, total);
  for (int x = 0; x < progress; x++) printf("#");
  for (int x = progress; x < width; x++) printf("-");
  printf("]");
  fflush(stdout);
}

static int _antenna_fwrite_fd(int antenna_method, int fd,
                              const char *file_path) {
  // Designate status
  int status = 0;

  // Get file size
  struct stat st;
  stat(file_path, &st);
  size_t file_size = st.st_size;

  // Open the file
  FILE *f = fopen(file_path, "r");
  if (f == NULL) {
    printf("[!] Failed to open file to send\n");
    return status;
  }

  // DEBUG
  // printf("[DEBUG] Sending file notice...\n");

  // Send notice of incoming file to destination
  char file_notice[FILE_NOTICE_LEN];
  sprintf(file_notice, FILE_NOTICE_FMT, file_size);
  if (antenna_method == ANTENNA_ENCODE_RS) {
    antenna_write_rs_fd(fd, file_notice, FILE_NOTICE_LEN);
  } else {
    antenna_write_fd(fd, file_notice, FILE_NOTICE_LEN);
  }
  // DEBUG
  // printf("[DEBUG] Done!\n");
  // printf("[DEBUG] Starting to buffer-copy file out...\n");

  // Read file into buffer and pass along to antenna
  char buffer[FILE_BUFFER_SIZE];
  char buffer_rs[RS_DATA_LEN];
  int bytes_read = 0;
  size_t total_bytes_read = 0;
  int eof = 0;
  while (!eof) {
    if (antenna_method == ANTENNA_ENCODE_RS) {
      bytes_read = fread(buffer_rs, sizeof(char), RS_DATA_LEN, f);
      if (antenna_write_rs_fd(fd, buffer_rs, bytes_read) == -1) {
        printf("[!] Failed to write encoded file data to antenna\n");
        status = -1;
        goto cleanup;
      }
    } else {
      bytes_read = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, f);

      // DEBUG
      // printf("[DEBUG] fread(%d bytes) -> antenna_write_fd(%d bytes)\n",
      // FILE_BUFFER_SIZE, bytes_read);

      if (antenna_write_fd(fd, buffer, bytes_read) == -1) {
        printf("[!] Failed to write file data to antenna\n");
        status = -1;
        goto cleanup;
      }
    }
    total_bytes_read += bytes_read;
    if (bytes_read < FILE_BUFFER_SIZE) eof = 1;

    // Display progress
    display_progress(total_bytes_read, file_size, PROGRESS_BAR_WIDTH, "Upload");
  }

  // DEBUG
  // printf("[DEBUG] Done!\n");
  // printf("[DEBUG] Cleaning up.\n");

cleanup:
  // Close the file
  fclose(f);

  return status;
}

/**
 * @brief Send file over the air.
 *
 * @param fd File descriptor to use
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_fd(int fd, const char *file_path) {
  return _antenna_fwrite_fd(ANTENNA_ENCODE_NONE, fd, file_path);
}

/**
 * @brief Send file over the air.
 *
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite(const char *file_path) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_fwrite_fd(uartfd, file_path);
}

/**
 * @brief Send file over the air using FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_rs_fd(int fd, const char *file_path) {
  return _antenna_fwrite_fd(ANTENNA_ENCODE_RS, fd, file_path);
}

/**
 * @brief Send file over the air using FEC.
 *
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_rs(const char *file_path) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_fwrite_rs_fd(uartfd, file_path);
}

static int _antenna_fread_fd(int antenna_mode, int fd, const char *file_path) {
  int status = 0;
  TIMEOUT_START();

  // DEBUG
  // printf("[DEBUG] Waiting for file notice...\n");

  // Wait for file notice
  size_t file_size = 0;
  int ready = 0;
  while (!ready) {
    char notice[FILE_NOTICE_LEN];
    if (antenna_mode == ANTENNA_ENCODE_RS) {
      if (antenna_read_rs_fd(fd, notice, FILE_NOTICE_LEN, READ_MODE_UNTIL) ==
          -1) {
        printf("[!] Failed to read from antenna\n");
        return -1;
      }
    } else {
      int res = 0;
      if ((res = antenna_read_fd(fd, notice, FILE_NOTICE_LEN,
                                 READ_MODE_UNTIL)) == -1) {
        printf("[!] Failed to read from antenna\n");
        return -1;
      }
      IF_TIMEOUT(res, return TIMEOUT_OCCURED);
    }

    // Check if file notice received
    if (notice[0] == 'F') {
      sscanf(notice, "F%u", &file_size);
      ready = 1;

      // DEBUG
      // printf("[i] File notice received! Incoming file size is %u bytes\n",
      //        file_size);

    } else {
      continue;
    }
  }
  // DEBUG
  // printf("[DEBUG] Done!\n");

  // Create file to store
  FILE *fp = fopen(file_path, "w");
  if (fp == NULL) {
    printf("[!] Failed to open file placeholder for incoming file\n");
    return -1;
  }

  printf("\n");

  // Write incoming data to file
  char buffer[FILE_BUFFER_SIZE];
  char buffer_rs[RS_DATA_LEN];
  size_t total_bytes_read = 0;
  int bytes_read = -1;
  int bytes_to_read = 0;
  int bytes_remaining = 0;
  while (total_bytes_read < file_size) {
    TIMEOUT_UPDATE();
    TIMEOUT_IF(READ_OP_TIMEOUT, goto cleanup);

    if (antenna_mode == ANTENNA_ENCODE_RS) {
      bytes_remaining = file_size - total_bytes_read;
      bytes_to_read =
          (bytes_remaining > RS_DATA_LEN) ? RS_DATA_LEN : bytes_remaining;
      if ((bytes_read = antenna_read_rs_fd(fd, buffer_rs, bytes_to_read,
                                           READ_MODE_UNTIL)) == -1) {
        printf("[!] Failed to read data from the antenna\n");
        status = -1;
        goto cleanup;
      }
      // NOTE: In case the incoming stream of file bytes is interrupted, delayed
      // or missing.
      // 1. The file has obvious not transmitted in its entirety, but most of it
      // could be there.
      // 2. Uploads to the satellite must be pristine; but downloads can be
      // partial.
      // For now, interpret missing data or read timeout as file failure
      IF_TIMEOUT(bytes_read, status = TIMEOUT_OCCURED; goto cleanup);

      // Handle no bytes read clause
      if (bytes_read == 0) {
        // Go back to the start and try again
        continue;
      }

      if (fwrite(buffer_rs, sizeof(char), bytes_read, fp) < bytes_read) {
        printf(
            "[*] Could not write any or full number of bytes to file. "
            "SKIPPING.\n");
        continue;
      }
    } else {
      bytes_remaining = file_size - total_bytes_read;
      bytes_to_read = (bytes_remaining > FILE_BUFFER_SIZE) ? FILE_BUFFER_SIZE
                                                           : bytes_remaining;
      // DEBUG
      // printf("[DEBUG] Reading %d more bytes...\n", bytes_to_read);
      if ((bytes_read = antenna_read_fd(fd, buffer, bytes_to_read,
                                        READ_MODE_UNTIL)) == -1) {
        printf("[!] Failed to read data from the antenna\n");
        status = -1;
        goto cleanup;
      }

      // NOTE: In case the incoming stream of file bytes is interrupted, delayed
      // or missing.
      // 1. The file has obvious not transmitted in its entirety, but most of it
      // could be there.
      // 2. Uploads to the satellite must be pristine; but downloads can be
      // partial.
      // For now, interpret missing data or read timeout as file failure
      IF_TIMEOUT(bytes_read, status = TIMEOUT_OCCURED; goto cleanup);

      // DEBUG
      // printf("[DEBUG] %d bytes read!\n", bytes_read);

      // Handle no bytes read clause
      if (bytes_read == 0) {
        // Go back to the start and try again
        continue;
      }

      if (fwrite(buffer, sizeof(char), bytes_read, fp) < bytes_read) {
        printf(
            "[*] Could not write any or full number of bytes to file. "
            "SKIPPING.\n");
        continue;
      }
    }

    total_bytes_read += bytes_read;

    // Display progress
    display_progress(total_bytes_read, file_size, PROGRESS_BAR_WIDTH,
                     "Download");

    // DEBUG
    // printf("[DEBUG] Remaining bytes: %d\n", bytes_remaining);
    // printf("\r[i] Total bytes read: %d", total_bytes_read);
  }
  printf("\n");

cleanup:
  fclose(fp);

  RETURN_IF_TIMEOUT();
  return status;
}

/**
 * @brief Receive file over the air.
 *
 * @param fd File descriptor to use
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error, TIMEOUT_OCCURED on timeout
 */
int antenna_fread_fd(int fd, const char *file_path) {
  return _antenna_fread_fd(ANTENNA_ENCODE_NONE, fd, file_path);
}

/**
 * @brief Receive file over the air.
 *
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error, TIMEOUT_OCCURED on timeout
 */
int antenna_fread(const char *file_path) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_fread_fd(uartfd, file_path);
}

/**
 * @brief Receive file over the air using FEC.
 *
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error
 */
int antenna_fread_rs(const char *file_path) {
  // Make sure file desc initialized
  if (uartfd == -1) {
    printf(
        "[!] Cannot read to unitialized fd. Make sure to run antenna_init() "
        "first\n");
    return -1;
  }

  return antenna_fread_rs_fd(uartfd, file_path);
}

/**
 * @brief Receive file over the air using FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error
 */
int antenna_fread_rs_fd(int fd, const char *file_path) {
  return _antenna_fread_fd(ANTENNA_ENCODE_RS, fd, file_path);
}