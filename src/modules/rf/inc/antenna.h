#ifndef LORIS_ANTENNA_H
#define LORIS_ANTENNA_H

// Feature macros
#define _BSD_SOURCE

// Project headers
#include "antenna_packet.h"
#include "modutil.h"

// Reed-solomon library
#include "correct.h"

// Standard C libraries
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

// Settings //
// UART
#define UART_SPEED B9600
#define UART_PARITY 0
#define TERMIOS_VMIN 0   // # of characters
#define TERMIOS_VTIME 1  // in 0.1s increments

// Reed-solomon
#define RS_BLOCK_LEN 255
#define RS_DATA_LEN 223
#define RS_NUM_ROOTS 16

// Antenna
#define MAX_TXT_FILE_SIZE 8191
#define MAX_READ_LEN 256
#define FILE_BUFFER_SIZE 64
#define FILE_NOTICE_FMT "F%u"
#define FILE_NOTICE_LEN 16
#define WRITE_DELAY 200000L
#define WRITE_BUFFER_SIZE 64
#define READ_OP_TIMEOUT 3  // in seconds

// Display
#define PROGRESS_BAR_WIDTH 40

enum { READ_MODE_UPTO, READ_MODE_UNTIL };

enum { ANTENNA_ENCODE_NONE, ANTENNA_ENCODE_RS };

/**
 * @brief Initializes the UART port for the antenna.
 *
 * @param path Sets the device path to configure.
 * @return 0 on success, 1 on error.
 */
int antenna_init(const char* path);

/**
 * @brief Writes bytes to the antenna.
 *
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 *
 * @return 0 on success, -1 on error
 */
int antenna_write(const char* data, size_t data_len);

/**
 * @brief Identical to antenna_write, but allows a custom file descriptor to be
 * specified.
 *
 * @param fd File descriptor to use.
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_fd(int fd, const char* data, size_t data_len);

/**
 * @brief Writes bytes to the antenna with Reed-solomon FEC
 *
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_rs(const char* data, size_t data_len);

/**
 * @brief Writes bytes to the antenna with Reed-solomon FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use.
 * @param data Array of bytes to send.
 * @param data_len Number of bytes from data to send.
 * @return 0 on success, -1 on error
 */
int antenna_write_rs_fd(int fd, const char* data, size_t data_len);

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
int antenna_read(char* buffer, size_t read_len, int read_mode);

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
int antenna_read_fd(int fd, char* buffer, size_t read_len, int read_mode);

/**
 * @brief Reads Reed-solomon encoded bytes from the antenna.
 *
 * @param buffer Output buffer array for incoming bytes.
 * @param read_len Read UP TO or block UNTIL this many bytes read.
 * @param read_mode Set to READ_MODE_UPTO or READ_MODE_UNTIL
 * @return number of bytes read or < 0 for error.
 */
int antenna_read_rs(char* buffer, size_t read_len, int read_mode);

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
int antenna_read_rs_fd(int fd, char* buffer, size_t read_len, int read_mode);

/**
 * @brief Send file over the air.
 *
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite(const char* file_path);

/**
 * @brief Send file over the air but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_fd(int fd, const char* file_path);

/**
 * @brief Send file over the air using FEC.
 *
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_rs(const char* file_path);

/**
 * @brief Send file over the air using FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to file to send.
 * @return 0 on success, -1 on error
 */
int antenna_fwrite_rs_fd(int fd, const char* file_path);

/**
 * @brief Receive file over the air.
 *
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error, TIMEOUT_OCCURED on timeout
 */
int antenna_fread(const char* file_path);

/**
 * @brief Receive file over the air but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error, TIMEOUT_OCCURED on timeout
 */
int antenna_fread_fd(int fd, const char* file_path);

/**
 * @brief Receive file over the air using FEC.
 *
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error
 */
int antenna_fread_rs(const char* file_path);

/**
 * @brief Receive file over the air using FEC but allows a custom
 * file descriptor to be specified.
 *
 * @param fd File descriptor to use
 * @param file_path Path to incoming file destination
 * @return 0 on success, -1 on error
 */
int antenna_fread_rs_fd(int fd, const char* file_path);

#endif