/**
 * ipc_packet.h 
 * 
 * purpose: define a structure which encloses IPC packets 
 * author:  alex amellal
 * 
 */

#ifndef CUBESAT_CORE_INCLUDE_IPC_PACKET_H
#define CUBESAT_CORE_INCLUDE_IPC_PACKET_H

#include "ipc_settings.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/**
 * Return flags 
 */

enum {
  IPC_PACKET_PARSE_FAILED = -100,
};

/**
 * Types
 */

typedef struct IPCPacket {
  char addr[NAME_LEN];
  char msg[MAX_MSG_LEN];
  size_t msg_len;
} ipc_packet_t;

/**
 * Methods
 */

// Returns a blank (but initialized) packet 
// RETURN packet
ipc_packet_t ipc_packet_new();

// Tests if packet is blank or not 
// RETURN true/false
bool ipc_packet_blank(ipc_packet_t packet);

// Adds a packet to a packet queue 
// RETURN index of added packet
int ipc_packet_add(ipc_packet_t *queue, size_t queue_len, ipc_packet_t packet);

// Removes a packet from a queue
void ipc_packet_rm(ipc_packet_t *queue, size_t queue_len, int index);

// Tests to see if queue empty 
// RETURN true/false on whether at least one packet is waiting
bool ipc_packet_waiting(ipc_packet_t *queue, size_t queue_len);

// Take the top packet from the queue and move the rest up one
// RETURN top packet (if exists) otherwise returns blank packet.
// NOTE: it is a good idea to check ipc_packet_waiting before popping.
ipc_packet_t ipc_packet_pop(ipc_packet_t *queue, size_t queue_len);

// Clears entire packet queue 
void ipc_packet_clear(ipc_packet_t *queue, size_t queue_len);

// Parses packet from raw socket data 
// RETURN parse status
// NOTE: overflow requires being at least MAX_PACKET_LEN in size to avoid overflows (irony)
int ipc_packet_parse(char *data, size_t data_len, ipc_packet_t *packet_out, char *overflow);

// Exports packet contents into string buffers 
// RETURN length of msg 
int ipc_packet_export(ipc_packet_t packet, char src_out[NAME_LEN], char *buffer, size_t buffer_len);

#endif // End of header guard