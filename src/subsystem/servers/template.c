/**
 * template.c
 * 
 * purpose: demonstrate configuration of a subsystem server 
 * author: alex amellal 
 * 
 */

#include "subsystem/servers/template.h"

void* template_server_start() {
  /**
   * All code needed to start your server goes here.
   * 
   * Specifically, the subsystem dock will handle your server the following way:
   *  1. Creates a unique thread for the start() function.
   *  2. Periodically checks for a successful or erronous exit.
   *  3. May suddenly terminate the thread used to initialize your server (see stop())
   * 
   * Things NOT to do:
   *  1. Do not throw errors, they won't be caught. 
   *  2. Do not quit the start method using 'return'. Instead use pthread_exit(ptr).
   *  3. Do not use pthread_exit with a local variable (value cannot be read).
   */
  
  printf("Starting template!\n");
  for(;;) {
    printf("running template...\n");
    sleep(1);
  }
}

void template_server_stop() {
  /**
   * All code needed to properly shutdown your server goes here.
   * 
   * Specifically, the subsystem dock will stop or restart your server the following way:
   *  1. Terminates the thread which initially ran the start() method.
   *  2. Runs the stop() method once and NOT as a separate thread.
   *  3. Depending on the desired outcome, may immediately initialize start() again
   *     or do nothing.
   * 
   * Things NOT to do:
   *  1. Do not throw errors, they won't be caught. 
   *  2. Do not quit the start method using 'return'. Instead use pthread_exit(ptr).
   *  3. Do not use pthread_exit with a local variable (value cannot be read).
   */
  printf("Stopping template!\n");
}

/**
 * This is where you define your subsystem server container.
 * 
 * It only requires 2 elements to be initialized:
 *  1. The start() method (see above).
 *  2. The stop() method (see above).
 * 
 * Once defined, declare it as an extern variable in the header you wish to 
 * dedicate to add your server to the subsystem dock.
 */
ServerContainer template_server = {
    .start = template_server_start,
    .stop  = template_server_stop,
};