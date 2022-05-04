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

#include "antenna_packet.h"

/**
 * @brief Generates a new packet at memory location p. Must be allocated
 * memory or undefined behavior will occur.
 *
 * @param p Pointer to allocated packet which will be initialized.
 * @return 0 = OK, -1 = ERR
 */
int antenna_packet_new(struct antenna_packet *p) {
  // Check for NULL pointers
  if (p == NULL) {
    printf("[!] Cannot initialize null packet\n");
    return -1;
  }

  // Initialize packet
  memcpy(p->start_delimiter, PACKET_DELIMITER, PACKET_DELIMITER_LEN);
  memset(p->data, 0, PACKET_DATA_LEN);
  memcpy(p->end_delimiter, PACKET_DELIMITER, PACKET_DELIMITER_LEN);

  // done
  return 0;
}