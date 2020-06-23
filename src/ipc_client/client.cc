/*
* client.cc
*
*   purpose: Uses the client API to connect to the IPC, C++ style.
*   author: alex amellal
*
*/

// DSS IPC client API
#include "ipc/client_api.h"

// DSS namespace
using namespace dss;

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <exception>

// C++ Standard Classes
using std::cout;
using std::endl;
using std::string;
using std::exception;

int main(int argc, char * argv[])
{
  // Check argc
  if(argc != 2)
  {
    cout << "Invalid number of arguments" << endl << "Try: ./client_cpp <name>" << endl;
    return -1;
  }

  // Get name from arg list
  string name = argv[1];

  try
  {
    // Initialize IPC client interface
    ipc::init(name);
  }
  catch(exception & e)
  {
    cout << "ERROR: " << e.what() << endl;
  }

  // done
  return 0;
}
