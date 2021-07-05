/**
 * @file client.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Client binary for the IPC for testing purposes
 * @version 0.1
 * @date 2021-07-02
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "subsysmod.h"

// Standard C libraries
#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const char *argp_program_version = "IPC test client 0.1";
const char *argp_program_bug_address = "<loris@alexamellal.com>";
static char doc[] =
    "Examples:\n\
1. Connect to the IPC as <name>: \n\
$ ./IPC_CLIENT.out <name>\n";

/* The options we understand. */
static struct argp_option options[] = {{0}};

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 \nARG2";

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[1]; /* arg1 */
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key) {
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) /* Too many arguments. */
        argp_usage(state);

      arguments->args[state->arg_num] = arg;

      break;

    // NOTE: only needed if minimum number of args after options
    case ARGP_KEY_END:
      if (state->arg_num < 1) /* Not enough arguments. */
        argp_usage(state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

// Default IPC callback
CALLBACK(standard) {
  if (((char *)data)[0] == '*') {
    printf("[i] Listener for %.*s has received an incoming message: %.*s\n", 1,
           (char *)data, msg_len, msg);
  } else {
    printf("[i] Listener for %.*s has received an incoming message: %.*s\n",
           NAME_LEN, (char *)data, msg_len, msg);
  }
}

int main(int argc, char *argv[]) {
  // Setup default values
  struct arguments arguments = {.args[0] = "xxx"};

  // Parse arguments
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // Connect to the IPC
  OK(ipc_connect(arguments.args[0]));
  char *name = arguments.args[0];

  // Main prompt
  printf(
      "####################################\n"
      "#             IPC Client           #\n"
      "####################################\n"
      "# Options:                         #\n"
      "# - Create a listener  (N)         #\n"
      "# - Start refresh loop (L)         #\n"
      "# - Receive a message  (R)         #\n"
      "# - Send a message     (S)         #\n"
      "# - Remove a listener  (D)         #\n"
      "# - Show listeners     (+)         #\n"
      "# - Quit               (Q)         #\n"
      "####################################\n");

  // Main loop
  for (;;) {
    bool quit = false;

    // Prompt input
    int ilen = 1;
    char input[ilen + 2];
    printf("%.*s >> ", NAME_LEN, name);
    fgets(input, ilen + 2, stdin);

    // Extract selection from input
    char selection = input[0];

    // Parse options
    switch (selection) {
      case 'N':
      case 'n':
        printf("[i] Creating a new listener...\n");

        // Get listener source name
        char src[NAME_LEN + 2];
        printf("[?] Enter the source to listen to: ");
        fgets(src, NAME_LEN + 2, stdin);

        // MALLOC WARNING
        // Don't forget to free() all of the sources when removing listeners
        char *src_storage = malloc(NAME_LEN);
        if (src_storage != NULL) {
          strcpy(src_storage, src);
        } else {
          strcpy(src_storage, "xxx");
        }

        // Create listener
        ON_FAIL(ipc_create_listener(src, standard, src_storage), quit = true;
                break);

        printf("[i] Listener for %.*s created.\n", NAME_LEN, src);

        printf("[i] Done.\n");
        break;
      case 'L':
      case 'l':
        // Get loop duration
        printf(
            "[?] How long would you like to run the loop? (in seconds) (0 = "
            "forever): ");
        char duration_in[7];
        fgets(duration_in, 7, stdin);
        int duration = atoi(duration_in);

        if (duration) {
          printf("[i] Starting refresh loop & running for %d seconds...\n",
                 duration);
        } else {
          printf(
              "[i] Start refresh loop & running indefinitely... ([CTRL+C] to "
              "quit)\n");
          duration = -1;
        }

        time_t start = time(NULL);
        while (time(NULL) - start < duration || duration == -1) {
          ON_FAIL(ipc_refresh(), quit = true; break);
        }

        printf("[i] Time alloted to refresh loop elapsed.\n");
        printf("[i] Done.\n");
        break;
      case 'R':
      case 'r':
        // Get receive source
        printf("[?] Set the receive source filter: ");
        char src_in[NAME_LEN + 2];
        fgets(src_in, NAME_LEN + 2, stdin);

        // Get receive timeout
        printf(
            "[?] Set the timeout for the message receipt (in seconds) (0 = no "
            "timeout): ");
        char timeout_in[7];
        fgets(timeout_in, 7, stdin);
        int timeout = atoi(timeout_in);

        if (timeout) {
          printf("[i] Listening for a message from %.*s for %d seconds...\n",
                 NAME_LEN, src_in, timeout);
        } else {
          printf("[i] Listening for a message from %.*s...\n", NAME_LEN,
                 src_in);
        }

        char msg[MAX_MSG_LEN];
        int msg_len;
        IF_TIMEOUT((msg_len = ipc_recv(src_in, msg, MAX_MSG_LEN, timeout)),
                   printf("[i] Read timed out.\n"));

        printf("[i] Message received: %.*s\n", msg_len, msg);
        printf("[i] Done.\n");
        break;
      case 'S':
      case 's':
        // Get destination
        printf("[?] Enter the destination name: ");
        char dest[NAME_LEN + 2];
        fgets(dest, NAME_LEN + 2, stdin);

        // Get message
        printf("[?] Enter the message to send: ");
        char msg_in[MAX_MSG_LEN + 2];
        fgets(msg_in, MAX_MSG_LEN + 2, stdin);

        // Null terminate instead of newline terminate
        for (int x = 0; x < MAX_MSG_LEN; x++) {
          if (msg_in[x] == '\n') msg_in[x] = '\0';
        }

        printf("[i] Message (%s) will be sent to %.*s\n", msg_in, NAME_LEN,
               dest);

        ON_FAIL(ipc_send(dest, msg_in, strlen(msg_in)), quit = true; break);

        printf("[i] Message sent.\n");
        printf("[i] Done.\n");
        break;

      case 'D':
      case 'd':
        printf("[?] Enter the source of the filter you'd like to remove: ");
        fgets(src_in, NAME_LEN + 2, stdin);

        printf("[i] Listener for (%.*s) will be removed...\n", NAME_LEN,
               src_in);

        ON_FAIL(ipc_remove_listener(src_in), quit = true; break);

        printf("[i] Listener has been removed.\n");
        printf("[i] Done.\n");
        break;

      case '+':
        printf("[i] List of active listeners:\n");

        MsgReqDib active_dibs[MAX_NUM_DIBS];
        int n_dibs = ipc_get_active_listeners(active_dibs);
        for (int x = 0; x < n_dibs; x++) {
          printf("    -> %d. %.*s\n", x + 1, NAME_LEN, active_dibs[x].name);
        }

        printf("[i] Done.\n");
        break;

      case 'Q':
      case 'q':
        printf("[i] Quitting client...\n");
        quit = true;
        break;
    }

    if (quit) break;
  }

  // Disconnect from the IPC
  OK(ipc_disconnect());

  printf("[i] Done.\n");

  return 0;
}