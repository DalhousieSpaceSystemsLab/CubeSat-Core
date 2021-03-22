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
#include <string>

// Standard C++ classes
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
void recv(string src, void (*callback)(string, void*));
void recv(string src, void (*callback)(string, void*), void* data);

}  // namespace async

/**
 * @brief Gracefully disconnect from the IPC.
 *
 */
void disconnect();

}  // namespace ipc
}  // namespace dss

#endif /* __CLIENT_API_HPP__ */
