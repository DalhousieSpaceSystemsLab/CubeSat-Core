/**
 * @file subsystem_module.hpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief C++ wrapper for the modular placeholder for subsystem modules
 * (container)
 * @version 0.1
 * @date 2021-03-27
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef __SUBSYSTEM_MODULE_HPP__
#define __SUBSYSTEM_MODULE_HPP__

// Standard C libraries
#include <pthread.h>

// Standard C++ libraries
#include <functional>
#include <iostream>

// Standard C++ classes
using std::cout;
using std::endl;

namespace dss {

class Module {
 public:
  pid_t pid;
  pthread_t mtid;
  char* stack;

  virtual void start();
  virtual void stop();
};

// struct Module {
//   pid_t pid;
//   pthread_t mtid;
//   char* stack;

//   static int start(void* data);
//   static int stop(void* data);
// };

}  // namespace dss

#endif  // End of header guard