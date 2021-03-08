/**
 * ipc_packet.c
 *
 * purpose: define a structure which encloses IPC packets
 * author:  alex amellal
 *
 */

#include "ipc_packet.h"

// Returns a blank (but initialized) packet
// RETURN packet
ipc_packet_t ipc_packet_new() {
  ipc_packet_t packet;
  memset(packet.addr, 0, NAME_LEN);
  memset(packet.msg, 0, MAX_MSG_LEN);
  packet.msg_len = 0;
  return packet;
}

// Tests if packet is blank or not
// RETURN true/false
bool ipc_packet_blank(ipc_packet_t packet) {
  ipc_packet_t blank = ipc_packet_new();
  if (memcmp(packet.addr, blank.addr, NAME_LEN) == 0 &&
      memcmp(packet.msg, blank.msg, MAX_MSG_LEN) == 0 &&
      packet.msg_len == blank.msg_len) {
    return true;
  }

  // test did not pass.
  // done
  return false;
}

// Adds a packet to a packet queue
// RETURN index of added packet
int ipc_packet_add(ipc_packet_t *queue, size_t queue_len, ipc_packet_t packet) {
  for (int x = 0; x < queue_len; x++) {
    if (ipc_packet_blank(queue[x])) {
      queue[x] = packet;
      return x;
    }
  }

  // could not find a blank packet to replace.
  return -1;
}

// Removes a packet from a queue
void ipc_packet_rm(ipc_packet_t *queue, size_t queue_len, int index) {
  if (0 <= index && index < queue_len) {
    queue[index] = ipc_packet_new();
  }
}

// Tests to see if queue empty
// RETURN true/false on whether at least one packet is waiting
bool ipc_packet_waiting(ipc_packet_t *queue, size_t queue_len) {
  for (int x = 0; x < queue_len; x++) {
    if (!ipc_packet_blank(queue[x])) {
      return true;
    }
  }

  // only blank packets found
  // done
  return false;
}

// Take the top packet from the queue and move the rest up one
// RETURN top packet (if exists) otherwise returns blank packet.
// NOTE: it is a good idea to check ipc_packet_waiting before popping.
ipc_packet_t ipc_packet_pop(ipc_packet_t *queue, size_t queue_len) {
  // Check if no packets waiting (nothing to pop)
  if (!ipc_packet_waiting(queue, queue_len)) return ipc_packet_new();

  ipc_packet_t packet = ipc_packet_new();
  for (int x = 0; x < queue_len; x++) {
    if (!ipc_packet_blank(queue[x])) {
      packet = queue[x];
      for (int y = x; y < queue_len - 1; y++) {
        queue[x] = queue[x + 1];
      }
      queue[queue_len - 1] = ipc_packet_new();

      break;
    }
  }

  // done
  return packet;
}

// Clears entire packet queue
void ipc_packet_clear(ipc_packet_t *queue, size_t queue_len) {
  for (int x = 0; x < queue_len; x++) {
    queue[x] = ipc_packet_new();
  }
}

// Parses packet from raw socket data
// RETURN parse status
// NOTE: overflow requires being at least MAX_PACKET_LEN in size to avoid
// overflows (irony)
int ipc_packet_parse(char *data, size_t data_len, ipc_packet_t *packet_out,
                     char *overflow) {
  // Identify key parsing delimiters (and their indices in the data array)
  int o_brackets[MAX_NUM_PACKETS], ob_x = 0;
  int c_brackets[MAX_NUM_PACKETS], cb_x = 0;
  for (int x = 0; x < data_len; x++) {
    if (data[x] == '<') {
      o_brackets[ob_x] = x;
      ob_x++;
      continue;
    } else if (data[x] == '>') {
      c_brackets[cb_x] = x;
      cb_x++;
      continue;
    }
  }
  int ob_len = ob_x;

  // Defines flag values to keep track of parser status
  enum states { at_addr, at_first_bracket, at_msg, invalidating };
  ob_x = 0;
  int parser = at_addr;
  ipc_packet_t packet;
  int addr_x = 0, msg_x = 0;
  int overflow_x = 0;
  bool packet_success = false;

  // Parse data
  for (int data_x = 0; data_x < data_len; data_x++) {
    if (parser == at_addr) {
      if (data[data_x] == '<' || data[data_x] == '>') {
        parser = invalidating;
        continue;
      } else {
        if (addr_x < NAME_LEN) {
          if (data[data_x] == ' ') continue;
          packet.addr[addr_x] = data[data_x];
          addr_x++;
        } else {
          parser = at_first_bracket;
          continue;
        }
      }
    } else if (parser == at_first_bracket) {
      if (data[data_x] != '<' && data[data_x] != ' ') {
        parser = invalidating;
        continue;
      } else if (data[data_x] == '<') {
        parser = at_msg;
        continue;
      }
    } else if (parser == at_msg) {
      if (data[data_x] == '<') {
        parser = invalidating;
        continue;
      } else if (data[data_x] == '>') {
        packet.msg_len = msg_x;
        overflow_x = data_x + 1;
        packet_success = true;
        break;
      } else {
        if (msg_x < MAX_MSG_LEN) {
          packet.msg[msg_x] = data[data_x];
          msg_x++;
        } else {
          packet.msg_len = MAX_MSG_LEN;
          overflow_x = data_x + 1;
          break;
        }
      }
    } else if (parser == invalidating) {
      // Move to next opening bracket and reset packet
      ob_x++;
      if (ob_x < ob_len) {
        data_x = o_brackets[ob_x] - NAME_LEN - 1;
        packet = ipc_packet_new();
        parser = at_addr;
      } else {
        // give up
        return IPC_PACKET_PARSE_FAILED;
      }
    }
  }

  if (packet_success) {
    *packet_out = packet;
    if (overflow_x + NAME_LEN + 2 < data_len) {
      int overflow_len = data_len - overflow_x;
      strncpy(overflow, &data[overflow_x], overflow_len);
      return overflow_len;
    }

    // done
    return 0;
  }

  // done, packet failed
  return IPC_PACKET_PARSE_FAILED;
}

// Exports packet contents into string buffers
// RETURN length of msg
int ipc_packet_export(ipc_packet_t packet, char src_out[NAME_LEN], char *buffer,
                      size_t max_buffer_len) {
  strncpy(src_out, packet.addr, NAME_LEN);
  int msg_len =
      packet.msg_len < max_buffer_len ? packet.msg_len : max_buffer_len;
  strncpy(buffer, packet.msg, msg_len);
  return msg_len;
}