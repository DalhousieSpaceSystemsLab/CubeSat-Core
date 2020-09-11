/**
 * exceptions.hpp
 * 
 * purpose: define custom exceptions for the C++ wrappers 
 * author:  alex amellal 
 * 
 */

// Standard C++ libraries 
#include <exception>

// Standard C++ classes 
using std::exception;

namespace dss {
  namespace ipc {
    class EInvalidName : public exception {
      const char * what() const throw() {
        return "Invalid name provided to connect to the IPC. Must be a 3 letter name.";
      }
    };

    class EConn : public exception {
      const char * what() const throw() {
        return "Error encountered with connection to the IPC server (daemon)";
      }
    };
  }
}