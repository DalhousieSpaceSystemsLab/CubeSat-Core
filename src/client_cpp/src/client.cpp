/**
 * @file client.cc
 * @author alex amellal (loris@alexamellal.com)
 * @brief C++ IPC client for testing purposes
 * @version 0.1
 * @date 2021-03-22
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

// Project headers
#include "client_api.hpp"

// Project classes
using namespace dss;

// Standard C++ libraries
#include <iostream>

// Standard C++ classes
using std::cout;
using std::endl;

void cb_read(string msg, void* data) {
  cout << "CB incoming message: " << msg << endl;
}

int main() {
  int a = 5;
  try {
    ipc::connect("alx");

    ipc::async::createListener("*", [](string msg) {
      cout << "This is callback message: " << msg << endl;
    });

    for (;;) {
      ipc::async::refresh();
    }

    ipc::disconnect();
  } catch (std::exception& e) {
    cout << "Caught error: " << e.what() << endl;
  }
}