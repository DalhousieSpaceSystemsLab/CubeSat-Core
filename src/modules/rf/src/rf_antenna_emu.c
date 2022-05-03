/**
 * @file rf_antenna_emu.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief RF Antenna emulation lib
 * @version 0.1
 * @date 2022-02-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#include "rf_antenna_emu.h"

static int fifo_rx = -1;
static int fifo_tx = -1;

/**
 * @brief Initialize RF antenna emulation module
 *
 * @param fifo_rx Path to named pipe (RX)
 * @param fifo_tx Path to named pipe (TX)
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_init(const char *fifo_rx, const char *fifo_tx) {
  // Create fifo devices
  if (mkfifo(FIFO_RX_NAME, 0666) < 0) {
    printf("[!] Failed to create RX fifo\n");
    return -1;
  }

  if (mkfifo(FIFO_TX_NAME, 0666) < 0) {
    printf("[!] Failed to create TX fifo\n");
    return -1;
  }

  if ((fifo_rx = open(FIFO_RX_NAME, 0666)) < 0) {
    printf("[!] Failed to open fifo_rx device\n");
    return -1;
  }

  if ((fifo_tx = open(FIFO_TX_NAME, 0666)) < 0) {
    printf("[!] Failed to open fifo_tx device\n");
    return -1;
  }

  // done
  return 0;
}

/**
 * @brief Checks whether the module has been initialized.
 *
 * @return 1 = true, 0 = false
 */
int rf_antenna_emu_is_init() {
  if (fifo_rx == -1 || fifo_tx == -1) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * @brief Write bytes to the antenna TX.
 *
 * @param data Array of bytes to send.
 * @param data_len Length of data to send.
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_write(const char *data, size_t data_len) {
  if (write(fifo_tx, data, data_len) < data_len) {
    printf("[!] Failed to write %d bytes of data to TX\n", data_len);
    return -1;
  } else {
    return 0;
  }
}

/**
 * @brief Send message.
 *
 * @param msg Message to send
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_send(const char *msg) {
  if (rf_antenna_emu_write(msg, strlen(msg)) < 0) {
    printf("[!] Failed to write message to antenna\n");
    return -1;
  } else {
    return 0;
  }
}

/**
 * @brief Read bytes from the antenna RX.
 *
 * @param data_in Array to store incoming bytes.
 * @param max_data_len Max size of data_in.
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_read(char *data_in, size_t max_data_len) {
  int bytes_read = -1;
  if ((bytes_read = read(fifo_rx, data_in, max_data_len)) < 0) {
    printf("[!] Failed to read data from the antenna\n");
    return -1;
  } else {
    return bytes_read;
  }
}

/**
 * @brief Receive message.
 *
 * @param buffer Buffer to receive incoming message.
 * @param max_len Max length buffer can handle.
 * @return # of bytes received or -1 for ERR
 */
int rf_antenna_emu_recv(char *buffer, size_t max_len) {
  return rf_antenna_emu_read(buffer, max_len);
}