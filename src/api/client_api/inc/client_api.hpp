/**
 * @file client_api.hpp
 * @author alex amellal (loris@alexamellal.com)
 * @brief C++ wrapper for the IPC client API
 * @version 0.1
 * @date 2021-03-22
 *
 * Dalhousie Space Systems Lab (c) 2021
 *
 */

#ifndef __CLIENT_API_HPP__
#define __CLIENT_API_HPP__

// Project headers
#include "client_api.h"

// Standard C++ libraries
#include <exception>
#include <string>

// Standard C++ classes
using std::exception;
using std::string;

// Dalhousie Namespace
namespace dss {

// IPC namespace
namespace ipc {
/**
 * @brief Connect to IPC daemon. Must be run before any IPC operations will
 * work.
 *
 * @param name 3 letter name used to register on the IPC network
 */
void connect(string name);

/**
 * @brief Sends string to destination process.
 *
 * @param dest 3 letter name belonging to destination process
 * @param msg String to send to dest.
 */
void send(string dest, string msg);

/**
 * @brief Receive incoming message from IPC.
 * **NOTE** this is a blocking type method; meaning it will not return until a
 * message has been received.
 *
 * @param src	Incoming message source filter. Use "*" for wildcard (any
 * source).
 * @return string containing incoming message
 */
string recv(string src);

namespace async {
/**
 * @brief Creates asynchronous background listener for incoming messages (with
 * callback).
 *
 * @param src	Incoming message source to monitor ("*" for wildcard)
 * @param callback Function to pass message to for processing.
 * @param data (Optional) Additional data for callback method.
 */
void recv(string src, void (*callback)(string, void*), void* data);
void recv(string src, void (*callback)(string, void*));

}  // namespace async

/**
 * @brief Gracefully disconnect from the IPC.
 *
 */
void disconnect();

namespace exception {
class InvalidDest : public exception {
 public:
  const char* what() const throw() {
    return "Destination name must be NAME_LEN characters long";
  }
};

class InvalidSrc : public exception {
 public:
  const char* what() const throw() {
    return "Source name be NAME_LEN characters long or wildcard (*)";
  }
};

class InvalidMsg : public exception {
 public:
  const char* what() const throw() {
    return "Message must be less than MAX_MSG_LEN characters long";
  }
};

class IPCAPI : public exception {
 public:
  const char* what() const throw() { return "Unknown IPC API error : "; }
};
}  // namespace exception

}  // namespace ipc
}  // namespace dss

#endif /* __CLIENT_API_HPP__ */
