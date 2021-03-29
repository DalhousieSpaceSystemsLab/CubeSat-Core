/**
 * @file template_module_cpp.hpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief Demo making a dock module in C++
 * @version 0.1
 * @date 2021-03-29
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef __TEMPLATE_MODULE_CPP_HPP__
#define __TEMPLATE_MODULE_CPP_HPP__

// Project headers
#include "client_api.hpp"
#include "module.hpp"

// Dalhousie Space Systems Lab Namespace
namespace dss {

// New module definiton
class TemplateModule : public Module {
  void start();
  void stop();
};

// static C-style functions for compatibility
int start_template_module(void* data);
int stop_template_module(void* data);

}  // namespace dss

#endif  // End of header guard