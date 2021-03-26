/**
 * @file f_c2cpp.cpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief support library for mingling C++ std::functions and C function
 * @version 0.1
 * @date 2021-03-26
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

// Project headers
#include "f_c2cpp.hpp"

std::function<void(string)> dss::fC2CPP::stdf;
std::function<void(string, void*)> dss::fC2CPP::stdf_d;
bool dss::fC2CPP::has_d;

dss::fC2CPP::fC2CPP(std::function<void(string)> f) {
  // Set stdf
  stdf = f;

  // Set data pointer status
  has_d = false;
}

dss::fC2CPP::fC2CPP(std::function<void(string, void*)> f) {
  // Set stdf_d
  stdf_d = f;

  // Set data pointer status
  has_d = true;
}

void dss::fC2CPP::run(char* msg, size_t msg_len, void* data) {
  if (has_d) {
    stdf_d(string(msg, msg_len), data);
  } else {
    stdf(string(msg, msg_len));
  }
}