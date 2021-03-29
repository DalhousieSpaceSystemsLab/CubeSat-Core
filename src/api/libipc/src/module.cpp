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

void dss::Module::start() {
  cout << "Default start method is running for this module... Did you override "
          "the start method yet?"
       << endl;
}

void dss::Module::stop() {
  cout << "Default stop method is running for this module... Did you override "
          "the stop method yet?"
       << endl;
}