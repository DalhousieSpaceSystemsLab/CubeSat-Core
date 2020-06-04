/*
* client_api.cc
*
*   purpose: provides API for other subsystems to use the IPC system as clients.
*   author: alex amellal
*
*/

// Project headers
#include "ipc/client_api.h"

using namespace dss;

// Exceptions
class e_bad_name : public exception
{
public:
  virtual const char * what() const throw() { return "name must be 3 alphanumeric characters. refusing to continue."; }
} err_bad_name;

class e_ipc_init_failed : public exception
{
public:
  virtual const char * what() const throw() { return "failed to initialize ipc client interface."; }
} err_ipc_init_failed;

// Initialize IPC client interface
void ipc::init(string name)
{
  // Check name length
  if(name.size() != 3) throw err_bad_name;

  // Convert name string into C string
  char c_name[3];
  strcpy(c_name, name.c_str());

  // Run C init
  if(ipc_init(c_name) == -1) // ipc_init() failed
  {
    throw err_ipc_init_failed;
  }
}
