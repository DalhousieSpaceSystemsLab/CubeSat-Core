/**
 * @file f_c2cpp.hpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief support library for mingling C++ std::functions and C function
 * pointers
 * @version 0.1
 * @date 2021-03-26
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef __F_C2CPP_HPP__
#define __F_C2CPP_HPP__

// Standard C++ libraries
#include <functional>
#include <string>

// Standard C++ classes
using std::string;

namespace dss {

class fC2CPP {
 public:
  // std::function with message string
  static std::function<void(string)> stdf;

  // std::function with message string and data pointer
  static std::function<void(string, void*)> stdf_d;

  // stores whether data pointer is used
  static bool has_d;

  fC2CPP(std::function<void(string)> f);
  fC2CPP(std::function<void(string, void*)> f);

  static void run(char* msg, size_t msg_len, void* data);
};

}  // namespace dss

#endif