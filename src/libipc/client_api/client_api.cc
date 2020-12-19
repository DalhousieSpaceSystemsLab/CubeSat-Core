/**
 * client_api.cc
 * 
 * purpose: C++ wrapper for client_api.c
 * author:  alex amellal 
 * 
 */

// Project header 
#include "ipc/client_api.hpp"

// Dalhousie Space Systems namespace 
using namespace dss;

// Placeholder for qrecv result
static char qrecv_buf[MAX_MSG_LEN];

// Message request 'dibs' list 
static MsgReqDib dibs[MAX_NUM_DIBS];

// Initialize client side IPC interface
void ipc::connect(string name) {
  // Verify length of name does not exceed name length 
  if(name.length() != NAME_LEN) {
    cerr << "Name provided to ipc::connect() is invalid" << endl;
    throw ipc::EInvalidName();
  }

  // Connect to the ipc 
  if(ipc_connect((char *) name.c_str()) != 0) {
    cerr << "Unable to connect to the IPC" << endl;
    throw ipc::EConn();
  }
}

// Send message to another process
void ipc::send(string dest, string msg) {
  // Make sure destination name is correct length 
  if(dest.length() != NAME_LEN) {
    cerr << "Destination name provided to ipc::send() is invalid" << endl;
    throw ipc::EInvalidName();
  }

  // Make sure message does not exceed max length 
  if(msg.length() >= MAX_MSG_LEN) {
    // Skip this message
    cerr << "Message too long. SKIPPING." << endl;
    return;
  }

  // Send message 
  if(ipc_send((char *) dest.c_str(), (char *) msg.c_str(), msg.length()) != 0) {
    cerr << "Unable to send message to the IPC. ipc::send() failed" << endl;
    throw ipc::EConn();
  }
}

// Receive message from another process (blocks)
string ipc::recv(string src) {
  // Make sure source filter matches name length
  if(src.length() != NAME_LEN && src.length() != 1) {
    cerr << "Name provided as source filter for ipc::recv() is invalid" << endl;
    throw ipc::EInvalidName();
  }

  // Create placeholder for incoming message 
  char msg[MAX_MSG_LEN];

  // Run ipc_recv 
  if(ipc_recv((char *) src.c_str(), msg, MAX_MSG_LEN) <= 0) {
    cerr << "Failed to receive message from IPC. ipc::recv() failed" << endl;
    throw ipc::EConn();
  }

  // Convert c_str into C++ string 
  string msg_cpp = msg;

  // done 
  return msg_cpp;
}

// Adds outgoing message to send queue
void ipc::async::send(string dest, string msg) {
  // Check if dest is the correct length 
  if(dest.length() != NAME_LEN) {
    throw ipc::EInvalidName();
  }

  // Check if message length does not exceed maximum 
  if(msg.length() > MAX_MSG_LEN) {
    throw ipc::EInvalidMsg();
  }

  // Add message to queue 
  if(ipc_qsend((char *) dest.c_str(), (char *) msg.c_str(), msg.length()) != 0) {
    cerr << "ipc_qsend() failed" << endl;
    throw ipc::EAPI();
  }

  // done 
  return;
}

// Adds incoming message request to recv queue (no callback)
void ipc::async::recv(string src) {
  // Check source name length 
  if(src.length() != NAME_LEN) {
    throw ipc::EInvalidName();
  }

  // Add message request to queue 
  if(ipc_qrecv((char *) src.c_str(), NULL, NULL) != 0) {
    cerr << "ipc_qrecv() failed" << endl;
    throw ipc::EAPI();
  }

  // done 
  return;
}

// Adds incoming message request to recv queue (with callback)
void ipc::async::recv(string src, void (*callback)(string)) {
  // Check source name length 
  if(src.length() != NAME_LEN) {
    throw ipc::EInvalidName();
  }


}

// Simultaneously reads/writes queued data
string ipc::async::refresh(string src) {
  // Refresh queued buffers 
  if(ipc_refresh() != 0) {
    cerr << "ipc_refresh() failed" << endl;
    throw ipc::EAPI();
  }

  // Check if qrecv buffer blank


  // done 
  return "abc";
}

// Close client side IPC interface
void ipc::disconnect() {
  // Disconnect from the ipc 
  ipc_disconnect();
}