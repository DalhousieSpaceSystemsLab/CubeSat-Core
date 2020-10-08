/**
 * client_api.hpp
 * 
 * purpose: C++ wrapper for client_api.h 
 * author:  alex amellal 
 * 
 */

extern "C" {
  #include "client_api.h"
}

// Project headers 
#include "exceptions.hpp"

// Standard C++ libraries 
#include <string>
#include <iostream>

// Standard C++ classes 
using std::string;
using std::cerr;
using std::endl;

// Client API 
namespace dss {
  namespace ipc {
    void connect(string name);            // Initialize client side IPC interface
    void send(string dest, string msg);   // Send message to another process
    string recv(string src);              // Receive message from another process (blocks)
    void disconnect();                    // Close client side IPC interface

    namespace async {
      void send(string dest, string msg); // Adds outgoing message to send queue
      void recv(string src);              // Adds message request to recv queue (non-blocking)
      void recv(string src, void (*callback)(string)); // Same as recv but with callback
      void refresh();                     // Simultaneously reads/writes all queued data and runs all callbacks
      string refresh(string src);         // Refreshes specific source-filtered queue and returns result.
    }
  }
}