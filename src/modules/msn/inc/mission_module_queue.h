/**
 * @file mission_module_queue.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Handles the queue for the mission module
 * @version 0.1
 * @date 2022-02-26
 *
 * @copyright Dalhousie Space Systems Lab (c) 2022
 *
 */

#ifndef CUBESAT_CORE_MODULES_MISSION_MODULE_QUEUE_H
#define CUBESAT_CORE_MODULES_MISSION_MODULE_QUEUE_H

// Project headers
#include "mission_module_global.h"

/**
 * @brief Add mission to queue
 *
 * @param msn
 * @return index if set, -1 otherwise
 */
int add_mission(struct mission mission_queue[static MAX_NUM_MISSIONS],
                struct mission msn);

/**
 * @brief Deletes the mission in the n'th position of the mission queue
 *
 * @param index
 * @param mission_queue
 * @return int
 */
int rm_mission(int index,
               struct mission mission_queue[static MAX_NUM_MISSIONS]);

/**
 * @brief Iterates though queued missions and checks to see if their conditions
 * have been met
 *
 * @return int
 */
int check_mission_queue(struct mission mission_queue[static MAX_NUM_MISSIONS]);

#endif