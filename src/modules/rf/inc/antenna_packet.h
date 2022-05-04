/**
 * @file antenna_packet.h
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Provide packet support for the antenna lib for loris
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef LORIS_ANTENNA_PACKET_H
#define LORIS_ANTENNA_PACKET_H

// Standard C library
#include <stdio.h>
#include <string.h>

// Settings
#define PACKET_DELIMITER "###"
#define PACKET_DELIMITER_LEN 3
#define PACKET_DATA_LEN 255

// Packet struct
struct antenna_packet {
  char start_delimiter[PACKET_DELIMITER_LEN];
  char data[PACKET_DATA_LEN];
  char end_delimiter[PACKET_DELIMITER_LEN];
};

/**
 * @brief Generates a new packet at memory location p. Must be allocated
 * memory or undefined behavior will occur.
 *
 * @param p Pointer to allocated packet which will be initialized.
 * @return 0 = OK, -1 = ERR
 */
int antenna_packet_new(struct antenna_packet *p);

#endif