/**
 * dock.c
 *
 * purpose: act as a containerized executable platform for subsystem servers
 * author: alex amellal
 *
 * TODO:
 *  - Rename header files to be unique (given rekt folder structure)
 *    duplicate dibs error to appear. Must not be removing dibs.
 *  - Make sure stack is large enough (edit stack size at startup)
 *  - Add timeout to stop_server function (around the wait() method)
 *
 */

#include "dock.h"

// Contains addresses of module memory stacks
static char* stacks[MAX_NUM_MODULES];

// Stops module by running module.stop()
// Uses provided stack as context for executing the stop method.
static int stop_module(SubsystemModule* module);

// Starts module by running module.run() and monitors process.
// Process will be restarted in case of exit.
static void* run_module(void* args);

// Attempts to stop process using interrupt signal within timeout
// If timeout exceeded and process has not terminated, SIGKILL is sent.
static void fstop(pid_t pid, int sec_timeout, struct timespec retry_delay);

// Start all server containers
int dock_start(SubsystemModule* modules, size_t modules_len) {
  // Ensure modules pointer is not null
  if (modules == NULL) {
    dockerr("modules pointer cannot be null : ");
    return -1;
  }

  // Ensure length is a non-zero positive value
  if (modules_len <= 0) {
    dockerr("modules array length must be a non-zero positive value : ");
    return -1;
  }

  // Initialize containers
  for (int x = 0; x < modules_len; x++) {
    // Allocate memory for stack
    if ((stacks[x] = (char*)malloc(MODULE_STACK_SIZE)) == NULL) {
      dockerr("failed to malloc() memory for stacks\n");
      return -1;
    }

    // Initialize module
    modules[x].pid = -1;
    modules[x].mtid = -1;
    modules[x].stack = stacks[x];

    // Start monitoring server
    if (pthread_create(&modules[x].mtid, NULL, run_module, &modules[x]) < 0) {
      perror("failed to pthred_create module monitor");
      return -1;
    }

    nanosleep(&MODULE_START_DELAY, NULL);
  }

  return 0;
}

// Stop all server containers
int dock_stop(SubsystemModule* modules, size_t modules_len) {
  // Shutdown all modules & monitors
  for (int x = 0; x < modules_len; x++) {
    // Stop module
    if (stop_module(&modules[x]) != 0) {
      dockerr("stop_module() failed : ");
      return -1;
    }

    // Free stack
    free(modules[x].stack);
  }

  // done
  return 0;
}

// Stop execution of server and run stop function associated to it
static int stop_module(SubsystemModule* module) {
  // Check if module pointer non null
  if (module == NULL) {
    dockerr("cannot use null pointer to module : ");
    return -1;
  }

  // Check if pid is initialized
  if (module->pid == -1) {
    fprintf(stdout, "module not running. NOT STOPPING.\n");
    return -1;
  }

  // Interrupt start process
  fstop(module->pid, MODULE_INT_TIMEOUT, MODULE_REINT_DELAY);

  // Reset pid value for module
  module->pid = -1;

  // Run stop function within process stack
  pid_t stop_pid = -1;
  if ((stop_pid = clone(module->stop, &module->stack[MODULE_STACK_SIZE],
                        SIGCHLD, NULL)) < 0) {
    perror("failed to clone() stop process");
  }

  // Wait for stop process to finish
  if (twaitpid(stop_pid, NULL, MODULE_STOP_TIMEOUT) == 0) {
    // Process did not stop on its own in time.
    // Force stop process using term/kill signals.
    fstop(stop_pid, MODULE_INT_TIMEOUT, MODULE_REINT_DELAY);
  }

  // Clear stack
  memset(module->stack, 0, MODULE_STACK_SIZE);

  // DEBUG
  dockprintf("successfully stopped module\n");

  // done
  return 0;
}

/**
 * Thread which continuously monitors for successful or erroneous exit
 * of designated server's 'start()' method
 */
static void* run_module(void* args) {
  // Convert void pointer args to SubsystemModule
  SubsystemModule* module = (SubsystemModule*)args;

  for (;;) {
    // Create start process
    if ((module->pid = clone(module->start, &module->stack[MODULE_STACK_SIZE],
                             SIGCHLD, NULL)) < 0) {
      perror("failed to clone() start process");
      pthread_exit(NULL);
    }

    // Wait on process to terminate
    int status = -1;
    waitpid(module->pid, &status, 0);

    // Check if process was terminated by a signal
    if (WIFSIGNALED(status)) {
      // DEBUG
      dockprintf("process terminated by signal! sig = %d\n", WTERMSIG(status));
      // Stop monitoring
      pthread_exit(NULL);
    }

    // Stop module
    if (stop_module(module) != 0) {
      dockerr("stop_module() failed\n");
      pthread_exit(NULL);
    }
  }
}

// Attempts to stop process using interrupt signal within timeout
// If timeout exceeded and process has not terminated, SIGKILL is sent.
static void fstop(pid_t pid, int sec_timeout, struct timespec retry_delay) {
  bool pexited = false, psigterm = false;
  time_t start, current, time_elapsed = 0;
  time(&start);
  while (time_elapsed < sec_timeout) {
    kill(pid, SIGINT);
    int stat;
    waitpid(pid, &stat, WNOHANG);

    pexited = WIFEXITED(stat);
    psigterm = WIFSIGNALED(stat);
    if (pexited || psigterm) break;

    nanosleep(&retry_delay, NULL);
    time(&current);
    time_elapsed = current - start;
  }

  // Check to see if start process needs to be force stopped
  if (!(pexited || psigterm)) {
    kill(pid, SIGKILL);
  }
}