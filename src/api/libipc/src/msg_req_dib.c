/**
 * msg_req_dib.c
 *
 * purpose: defines the placeholder type for message source 'dibs'
 * author:  alex amellal
 *
 */

// Project header
#include "msg_req_dib.h"

// Returns blank dib
MsgReqDib MsgReqDib_new() {
  return (MsgReqDib){.name = {0, 0, 0}, .callback = NULL, .tid = -1};
}

// Returns initialized dib
MsgReqDib MsgReqDib_set(char name[NAME_LEN],
                        int (*callback)(char *, size_t, void *), void *data) {
  MsgReqDib dib = {.callback = callback, .data = data, .tid = -1};
  strncpy(dib.name, name, NAME_LEN);

  // done
  return dib;
}

// Appends or overwrites dib into dib array
// Returns index value where dib was added, otherwise -1 in case of error.
int MsgReqDib_add(MsgReqDib element, MsgReqDib *array, size_t array_len) {
  // Check for existing element with same name
  for (int x = 0; x < array_len; x++) {
    // Check if name matches
    if (strncmp(array[x].name, element.name, NAME_LEN) == 0) {
      // Overwrite existing element
      array[x] = element;

      // done
      return x;
    }
  }

  // No match found, place into first open spot
  for (int x = 0; x < array_len; x++) {
    // Create placeholder for blank dib
    MsgReqDib blank = MsgReqDib_new();

    // Check if blank dib
    if (strncmp(array[x].name, blank.name, NAME_LEN) == 0 &&
        array[x].callback == blank.callback) {
      // Place element into array
      array[x] = element;

      // done
      return x;
    }
  }

  // No match and no vacant place found, error
  fprintf(stderr,
          "no matching or vacant dib element found in dib array. unable to "
          "add. : ");
  return -1;
}

// Checks in array for preexisting dibs
bool MsgReqDib_exists(char name[NAME_LEN], MsgReqDib *array, size_t array_len) {
  // Search for existing dibs
  bool msg_has_dibs = false;
  for (int x = 0; x < array_len; x++) {
    // Check if name matches dib in question
    if (strncmp(array[x].name, name, NAME_LEN) == 0) {
      // Matching dib found, stop looking
      msg_has_dibs = true;
      break;
    }
  }

  // done
  return msg_has_dibs;
}

// Checks if callback for dib is running
bool MsgReqDib_is_running(MsgReqDib dib) {
  if (dib.tid == -1) {
    return false;
  } else {
    return true;
  }
}

// Removes a dib from an array of dibs
int MsgReqDib_remove(char name[NAME_LEN], MsgReqDib *array, size_t array_len) {
  // Parse through elements in array
  for (int x = 0; x < array_len; x++) {
    // Check if name matches name in dib
    if (strncmp(name, array[x].name, NAME_LEN) == 0) {
      // 'delete' element (really just clear it out)
      array[x] = MsgReqDib_new();

      // NOTE:
      // The for loop intentionally does not 'break' on the first
      // match since it could be useful to remove all matches when
      // more than one dib was created for a particular source.
    }
  }

  // done
  return 0;
}