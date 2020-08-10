/*
* test_client_api.c
*
*   purpose: Unit tests for the IPC daemon.
*   author: alex amellal
*
*/

#include "ipc/tests.h"

private const int accept_sock = 11;

private const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket"
  };
private const socklen_t address_len = sizeof(address);

void test_ipcd_init()
{
  // Configure wrappers //
  // socket() 
  will_return(__wrap_socket, AF_UNIX);
  will_return(__wrap_socket, SOCK_STREAM);
  will_return(__wrap_socket, 0);
  will_return(__wrap_socket, accept_sock);

  // unlink()
  will_return(__wrap_unlink, "./socket.socket");
  will_return(__wrap_unlink, 0);

  // bind()
  will_return(__wrap_bind, accept_sock);
  will_return(__wrap_bind, &address);
  will_return(__wrap_bind, address_len);
  will_return(__wrap_bind, 0);

  // listen()
  will_return(__wrap_listen, accept_sock);
  will_return(__wrap_listen, MAX_NUM_CLI);
  will_return(__wrap_listen, 0);

  // pthread_create()
  will_return(__wrap_pthread_create, NULL);
  will_return(__wrap_pthread_create, 0);
  will_return(__wrap_pthread_create, 0);

  // pthread_detach()
  will_return(__wrap_pthread_detach, 0);

  // run function 
  assert_int_equal(ipcd_init(), 0);
}

void test_ipcd_close()
{
  // Configure wrappers //
  // close()
  will_return(__wrap_close, accept_sock);
  will_return(__wrap_close, 0);

  // run function 
  assert_int_equal(ipcd_close(), 0);
}