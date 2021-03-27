/**
 * @file subsystem_module.cpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief C++ wrapper for the modular placeholder for subsystem modules
 * @version 0.1
 * @date 2021-03-27
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

// Project headers
#include "module.hpp"

int dss::Module::start(void* data) {
  cout << "Default start method is running for this module... Did you override "
          "the start method yet?"
       << endl;

  return 0;
}

int dss::Module::stop(void* data) {
  cout << "Default stop method is running for this module... Did you override "
          "the stop method yet?"
       << endl;

  return 0;
}