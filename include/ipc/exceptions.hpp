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
        return "Invalid name provided to connect to the IPC. Must be NAME_LEN characters long.";
      }
    };

    class EConn : public exception {
      const char * what() const throw() {
        return "Error encountered with connection to the IPC server (daemon)";
      }
    };

    class EInvalidMsg : public exception {
      const char * what() const throw() {
        return "Provided message is incompatible with IPC standards. Cannot exceed MAX_MSG_LEN characters.";
      }
    };

    class EAPI : public exception {
      const char * what() const throw() {
        return "Error encountered in underlying C api. Pay attention to other error messages to diagnose.";
      }
    };
  }
}