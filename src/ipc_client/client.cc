/**
 * client.cc
 * 
 * purpose: C++ wrapper for client.c 
 * author:  alex amellal 
 * 
 * TODO: 
 *  - Make C++ example ipc client
 */

// Project headers 
#include "ipc/client_api.hpp"

// Project namespace 
using namespace dss;

// Standard C++ libraries
#include <exception>
#include <iostream>

// Standard C++ classes
using std::exception;
using std::cout;
using std::endl;
using std::string;
using std::cin;

int main(int argc, char * argv[]) {
  // Check number of args 
  if(argc != 3) {
    cout << "Invalid number of arguments. Try ./client_cc <name> <read/write>" << endl;
    return -1;
  }

  // Sort argments into respective strings 
  string name = argv[1];
  string rdwr = argv[2];

  try {
    // Connect to the IPC 
    ipc::connect(name);

    // Check if client desires to read messages
    if(rdwr == "read") {
      for(;;) {
        // Wait for incoming message 
        string msg = ipc::recv("*");

        // Print message to screen 
        cout << "Incoming message: " << msg << endl;
      }
      // Check if client wishes to write messages
    } else if(rdwr == "write") {
      for(;;) {
        // Create placeholders for destination client name and message
        string dest;
        string msg;
        
        // Prompt user for destination name
        cout << "Enter the name of the destination client: ";
        std::getline(cin, dest);

        // Prompt user for message 
        cout << "Enter the message you wish to send: ";
        std::getline(cin, msg);

        // Send message to destination 
        ipc::send(dest, msg);

        // Add some space 
        cout << endl;
      }
    }

    // Disconnect from the IPC 
    ipc::disconnect();
    
  } catch(exception & e) {
    cout << e.what() << endl;
  }

  return 0;
}