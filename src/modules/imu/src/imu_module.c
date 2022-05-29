/**
 * @file imu_module.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief IMU support for LORIS
 * @version 0.1
 * @date 2022-05-29
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "imu_module.h"

START_MODULE(imu) {
  // Configure UART fd
  int uartfd;
  if ((uartfd = open(IMU_DEV_PATH, O_RDWR | O_NOCTTY | O_SYNC)) < 0) {
    moderr("[!] Failed to open I/O device at %s\n", IMU_DEV_PATH);
    return -1;
  }

  struct termios tty;

  if (tcgetattr(uartfd, &tty) < 0) {
    moderr("Error from tcgetattr: %s\n", strerror(errno));
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
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 5;

  if (tcsetattr(uartfd, TCSANOW, &tty) != 0) {
    moderr("Error from tcsetattr: %s\n", strerror(errno));
    return -1;
  }

  // Main loop
  for (;;) {
    FILE* fp = fopen(IMU_DATA_FILE, "w");
    if (!fp) {
      moderr("Failed to open IMU data file at %s\n", IMU_DATA_FILE);
      sleep(1);
      continue;
    }

    // Read entry from IMU
    char entry[MAX_ENTRY_LEN];
    int bytes_read = 0;
    if ((bytes_read = read(uartfd, entry, MAX_ENTRY_LEN)) < 0) {
      moderr("Failed to read IMU data from colibri-uartc\n");
      return -1;
    }

    // Write entry to file
    fwrite(entry, sizeof(char), bytes_read, fp);

    // done
    fclose(fp);
  }
}

STOP_MODULE(imu) {}

EXPORT_MODULE(imu);