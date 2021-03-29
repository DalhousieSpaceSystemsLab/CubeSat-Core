/**
 * @file template_module_cpp.cpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief Demo making a dock module in C++
 * @version 0.1
 * @date 2021-03-29
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#include "template_module_cpp.hpp"

void dss::TemplateModule::start() {
  /**
   * This is the start() method of your module.
   *
   * Anytime the module is initialized and run, this method is started and kept
   * alive for the duration of your module's existence.
   *
   * Any initializtion, connection, etc would go here. Also, any main loops need
   * to at least start here.
   *
   */
}

void dss::TemplateModule::stop() {
  /**
   * This is the stop() method of your module.
   *
   * Anytime the start() method quits or the module needs to be shutdown, the
   * dock will run this method once.
   *
   * If the stop method does not terminate within MODULE_STOP_TIMEOUT seconds,
   * it will be forcefully terminated.
   *
   * Any deinitializations, disconnections, deallocations, etc would go here.
   * Proper clean up is necessary to avoid memory leaks.
   *
   */
}

int dss::start_template_module(void* data) {
  TemplateModule mod;
  mod.start();
  return 0;
}

int dss::stop_template_module(void* data) {
  TemplateModule mod;
  mod.stop();
  return 0;
}