/**
 * @file rf_antenna_emu.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief RF Antenna emulation lib
 * @version 0.1
 * @date 2022-02-27
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_RF_ANTENNA_EMU_H
#define CUBESAT_CORE_MODULES_RF_ANTENNA_EMU_H

// Standard C libraries
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

// Settings
#define FIFO_RX_NAME "antenna_emu_fifo_rx"
#define FIFO_TX_NAME "antenna_emu_fifo_tx"

/**
 * @brief Initialize RF antenna emulation module
 *
 * @param fifo_rx Path to named pipe (RX)
 * @param fifo_tx Path to named pipe (TX)
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_init(const char *fifo_rx, const char *fifo_tx);

/**
 * @brief Checks whether the module has been initialized.
 *
 * @return 1 = true, 0 = false
 */
int rf_antenna_emu_is_init();

/**
 * @brief Write bytes to the antenna TX.
 *
 * @param data Array of bytes to send.
 * @param data_len Length of data to send.
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_write(const char *data, size_t data_len);

/**
 * @brief Send message.
 *
 * @param msg Message to send
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_send(const char *msg);

/**
 * @brief Read bytes from the antenna RX.
 *
 * @param data_in Array to store incoming bytes.
 * @param max_data_len Max size of data_in.
 * @return 0 = OK, -1 = ERR
 */
int rf_antenna_emu_read(char *data_in, size_t max_data_len);

/**
 * @brief Receive message.
 *
 * @param buffer Buffer to receive incoming message.
 * @param max_len Max length buffer can handle.
 * @return # of bytes received or -1 for ERR
 */
int rf_antenna_emu_recv(char *buffer, size_t max_len);

#endif