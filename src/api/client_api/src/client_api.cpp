/**
 * @file client_api.cc
 * @author alex amellal (loris@alexamellal.com)
 * @brief
 * @version 0.1
 * @date 2021-03-22
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

// Project headers
#include "client_api.hpp"

// Standard C++ libraries
#include <iostream>

using std::cout;
using std::endl;

// Connect to IPC daemon. Must be run before any IPC operations will work.
void dss::ipc::connect(string name) {
  if (ipc_connect(name.c_str()) < 0) {
    throw dss::ipc::exceptions::IPCAPI();
  }
}

// Sends string to destination process.
void dss::ipc::send(string dest, string msg) {
  if (dest.size() != NAME_LEN) {
    throw dss::ipc::exceptions::InvalidDest();
  }

  if (msg.size() >= MAX_MSG_LEN) {
    throw dss::ipc::exceptions::InvalidMsg();
  }

  if (ipc_send((char*)dest.c_str(), (char*)msg.c_str(), msg.size()) < 0) {
    cout << "Failed to send message to " << dest << endl;
    throw dss::ipc::exceptions::IPCAPI();
  }
}

// Receive incoming message from IPC.
string dss::ipc::recv(string src) {
  if (src.size() != NAME_LEN && src != "*") {
    cout << "Destination name must be " << NAME_LEN << "characters long"
         << endl;
    throw dss::ipc::exceptions::InvalidSrc();
  }

  char msg[MAX_MSG_LEN];
  if (ipc_recv((char*)src.c_str(), msg, MAX_MSG_LEN) <= 0) {
    cout << "Failed to receive incoming message from IPC" << endl;
    throw dss::ipc::exceptions::IPCAPI();
  }

  return string(msg);
}

// Gracefully disconnect from the IPC.
void dss::ipc::disconnect() { ipc_disconnect(); }