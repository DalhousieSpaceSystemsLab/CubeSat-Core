/*
 * client.c
 *
 *   purpose: Uses the client API to connect to the IPC.
 *   author: alex amellal
 *
 */

// Project headers
#include "client_api.h"

// Standard C libraries
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

// Interrupt signal routine handler
void isr(int sig);

// Callbacks for async communication
static void cb_read(char* msg, size_t msg_len, void* data);

int main(int argc, char* argv[]) {
  // Check argc
  if (argc != 3) {
    fprintf(stderr,
            "Invalid number of arguments\n Try: ./client <name> "
            "<read/write/async> (async is experimental)\n");
    return -1;
  }

  // Create placeholder for client name
  char* name = argv[1];
  char* rdwr = argv[2];

  // Copy read/write setting into formatted array
  char rdwr_fmt[5];
  strcpy(rdwr_fmt, rdwr);

  // Format rdwr real quick
  if (strcmp(rdwr, "read") == 0) {
    rdwr_fmt[4] = ' ';
  }

  // Check name length
  if (strlen(name) != 3) {  // name is not correct length
    fprintf(stderr, "name must be 3 characters long\n");
    return -1;
  }

  // Connect to IPC system
  if (ipc_connect(name) == -1) {  // ipc_connect() failed
    fprintf(stderr, "Failed to connect to the ipc\n");
    return -1;
  }

  // Welcome prompt
  printf(
      "\
  ################################################\n\
  #           IPC client example program         #\n\
  ################################################\n\
  #                  Settings                    #\n\
  #  Name: %.3s                                   #\n\
  #  Mode: %.5s                                 #\n\
  ################################################\n\
  #                                              #\n\
  #     Press [CTRL + C] at any time to quit     #\n\
  #                                              #\n\
  ################################################\n\n",
      name, rdwr_fmt);

  // Check if client reading or writing
  if (strcmp(rdwr, "read") == 0) {  // reading
    printf("[waiting for incoming messages...]\n\n");

    for (;;) {
      // Create placeholder for incoming message
      char src[NAME_LEN + 2];
      char msg[MAX_MSG_LEN];

      // Ask user for source filter
      // printf("Please enter the desired source filter (* for wildcard): ");
      // fgets(src, NAME_LEN+2, stdin);

      // Read data
      int bytes_read = -1;
      if ((bytes_read = ipc_recv("*", msg, MAX_MSG_LEN)) ==
          -1) {  // ipc_recv() failed
        fprintf(stderr, "ipc_recv() failed\n");
        return -1;
      }

      // Print data
      printf("message received: %s\n", msg);
    }
  }

  else if (strcmp(rdwr, "write") == 0) {  // writing
    for (;;) {
      // Create placeholders for message to send
      char dest[NAME_LEN + 2];
      char msg[MAX_MSG_LEN + 2];

      // Ask user for message and destination
      printf("Enter destination's name: ");
      fgets(dest, NAME_LEN + 2, stdin);

      // Ask user for message
      printf("Enter message: ");
      fgets(msg, MAX_MSG_LEN + 2, stdin);

      // Check if both entries are the [ENTER] key
      if (strncmp(dest, "\n", 1) == 0 && strncmp(msg, "\n", 1) == 0) {
        // quit
        break;
      }

      // Check if message 'quit'
      if (strcmp(msg, "quit") == 0) {  // user is asking to quit
        break;
      }

      // Send message to other client
      if (ipc_send(dest, msg, strlen(msg)) == -1) {  // ipc_send() failed
        fprintf(stderr, "ipc_send() failed\n");
        return -1;
      }
    }
  }

  else if (strcmp(rdwr, "async") == 0) {  // async communication
    // Create placeholder for dibs
    char dibs[MAX_NUM_DIBS][NAME_LEN + 2];
    size_t dibs_len = 0;

    // Get message dibs names from client
    for (int x = 0; x < MAX_NUM_DIBS; x++) {
      // Ask user to enter dib
      printf("Enter a message source you'd like dibs on ([ENTER] when done): ");
      fgets(dibs[x], NAME_LEN + 2, stdin);

      // Check if user pressed enter
      if (dibs[x][0] == '\n') break;

      // Increment dibs counter
      dibs_len++;
    }

    // Create message dibs in IPC
    for (int x = 0; x < dibs_len; x++) {
      // Check if ipc_qrecv failed
      if (ipc_qrecv(dibs[x], cb_read, NULL, IPC_QRECV_MSG) != 0) {
        fprintf(stderr, "ipc_qrecv() failed\n");
        return -1;
      }
    }

    // Loop on demand. Give user choice between refresh and send msg. //

    // Prompt instructions
    printf("### Message Loop ###\n");
    printf("Enter [r] to refresh incoming messages.\n");
    printf("Enter [s] to send a message\n");
    printf("Enter [d] to add a dib\n");
    printf("Enter [j] to send key-value pairs\n");
    printf("Press [ENTER] to quit\n\n");

    for (;;) {
      // Create placeholder for input
      char ans[1 + 2];

      // Prompt user for input
      printf("> ");
      fflush(stdout);

      // Get input
      fgets(ans, 1 + 2, stdin);

      // Check if enter, refresh or send
      if (ans[0] == '\n') {
        // Quit
        break;
      } else if (ans[0] == 'r') {
        // Refresh
        if (ipc_refresh() != 0) {
          fprintf(stderr, "ipc_refresh() failed\n");
          return -1;
        }
      } else if (ans[0] == 's') {
        // Create placeholder for message
        char msg[MAX_MSG_LEN + 2];

        // Get message from user
        printf(">> ");
        fgets(msg, MAX_MSG_LEN + 2, stdin);

        // Remove newline character
        for (int x = 0; x < MAX_MSG_LEN; x++) {
          if (msg[x] == '\n') {
            for (int y = x; y < MAX_MSG_LEN + 1; y++) {
              msg[y] = msg[y + 1];
            }
          }
        }

        // Separate name from msg
        char name[NAME_LEN];
        char msg_nameless[MAX_MSG_LEN];
        strncpy(name, msg, NAME_LEN);
        strncpy(msg_nameless, &msg[NAME_LEN + 1], MAX_MSG_LEN - (NAME_LEN + 1));

        // Send message
        if (ipc_send(name, msg_nameless, strlen(msg_nameless)) != 0) {
          fprintf(stderr, "ipc_send() failed\n");
          return -1;
        }
      } else if (ans[0] == 'd') {
        // Create placeholder for dibs dest
        char src[NAME_LEN + 2];

        // Get src name from user
        printf("[dib src] >> ");
        fgets(src, NAME_LEN + 2, stdin);

        // Add dib
        if (ipc_qrecv(src, cb_read, NULL, IPC_QRECV_MSG) != 0) {
          fprintf(stdout, "ipc_qrecv() failed\n");
          return -1;
        }

        // Confirm
        printf("[i] Dib successfully added\n\n");

      } else if (ans[0] == 'j') {
        char dest[NAME_LEN + 2];
        int max_kv = 5, total_kv = 0;
        json_t kv_pairs[max_kv];

        printf("[dest]> ");
        fgets(dest, NAME_LEN + 2, stdin);

        printf("Press [ENTER] to stop entering key-value pairs\n");
        for (int x = 0; x < max_kv; x++, total_kv++) {
          printf("[key]> ");
          fgets(kv_pairs[x].key, JSON_KEY_LEN, stdin);
          printf("[value]> ");
          fgets(kv_pairs[x].val, JSON_VAL_LEN, stdin);

          if (kv_pairs[x].key[0] == '\n' || kv_pairs[x].val[0] == '\n') break;

          for (int y = 0; y < JSON_KEY_LEN; y++) {
            if (kv_pairs[x].key[y] == '\n') kv_pairs[x].key[y] = '\0';
          }
          for (int y = 0; y < JSON_VAL_LEN; y++) {
            if (kv_pairs[x].val[y] == '\n') kv_pairs[x].val[y] = '\0';
          }
        }

        if (ipc_send_json(dest, kv_pairs, total_kv) < 0) {
          fprintf(stderr, "ipc_send_json() failed\n");
          return -1;
        }

      } else {
        // Not a recognized option, skip
        continue;
      }
    }
  } else if (strcmp(rdwr, "json") == 0) {
    // Create JSON data
    json_t data[] = {{.key = "name", .val = "alex"},
                     {.key = "gender", .val = "alpha male"},
                     {.key = "age", .val = "19"}};
    size_t data_len = sizeof(data) / sizeof(json_t);

    // Send over IPC
    if (ipc_send_json("alx", data, data_len) < 0) {
      fprintf(stderr, "ipc_send_json() failed\n");
      return -1;
    }

    // Stringify JSON
    char json_str[128];
    int json_str_len = 0;
    if ((json_str_len = json_stringify(data, data_len, json_str, 128)) < 0) {
      fprintf(stderr, "json_stringify() failed\n");
      return -1;
    }

    // Test json
    bool json_pass = json_test(json_str, json_str_len);
    printf("[i] json_test results: %s\n", json_pass ? "true" : "false");

    // Wait a sec
    sleep(1);

  } else if (strcmp(rdwr, "packet") == 0) {
    char data[MAX_PACKET_LEN] =
        "alx < waddup man this is cool > alx < here is another message >";
    size_t data_len = strlen(data);
    char overflow[MAX_PACKET_LEN];
    int overflow_len = -1;
    ipc_packet_t packet;

    while (overflow_len) {
      if ((overflow_len = ipc_packet_parse(data, data_len, &packet, overflow)) <
          0) {
        fprintf(stderr, "ipc_packet_parse() failed\n");
        return -1;
      }
      printf("[i] packet successfully parsed:\n");
      printf("[addr] = %.*s , [msg] = %.*s\n", NAME_LEN, packet.addr,
             packet.msg_len, packet.msg);
      printf("[overflow] = %.*s\n\n", overflow_len ? overflow_len : 4,
             overflow_len ? overflow : "none");

      strncpy(data, overflow, overflow_len);
      data_len = overflow_len;
    }

  } else {  // bad keyword
    fprintf(stderr, "invalid setting. try ./client <name> <read/write>\n");
    return -1;
  }

  // printf("presss [ENTER] to quit\n");
  // fgetc(stdin);

  // Close IPC
  ipc_disconnect();

  // done
  return 0;
}

// Interrupt signal routine
void isr(int sig) {
  switch (sig) {
    case SIGINT:
      ipc_disconnect();
      exit(0);
      break;
  }
}

// Callback for async communication
static void cb_read(char* msg, size_t msg_len, void* data) {
  // Check if incoming message is a key-value pair
  if (json_test(msg, msg_len)) {
    int max_kv = 5;
    json_t json[max_kv];
    int kv_parsed = 0;
    if ((kv_parsed = json_parse(msg, msg_len, json, max_kv)) < 0) {
      fprintf(stderr, "json_parse() failed\n");
      return;
    }
    for (int x = 0; x < kv_parsed; x++) {
      printf("[pair %d] %s : %s\n", x, json[x].key, json[x].val);
    }
    return;
  }

  char args[10][MAX_ARG_LEN];
  int argc = ipc_args(msg, msg_len, args, 10);

  // Print message
  printf("Number of args = %d\n", argc);
  for (int x = 0; x < argc; x++) printf("arg[%d] = %s, ", x, args[x]);
  printf("\n");
  printf("Incoming message: %.*s\n", msg_len, msg);
}