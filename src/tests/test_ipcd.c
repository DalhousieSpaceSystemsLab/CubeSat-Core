/*
* test_client_api.c
*
*   purpose: Unit tests for the IPC daemon.
*   author: alex amellal
*
*/

#include "ipc/tests.h"
#include "../libipc/ipcd/ipcd.c"

private const int accept_sock = 11;
 
private const client_t expected_client = {
  .conn = {
    .rx = 22,
    .tx = 23,
  },
  .name = "pwr"
};

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

void test_ipcd_start_accepting()
{
  // Configure wrappers //

  // Connect first time //
  // accept() -> new client 
  will_return(__wrap_accept, accept_sock);
  will_return(__wrap_accept, NULL);
  will_return(__wrap_accept, NULL);
  will_return(__wrap_accept, expected_client.conn.rx);

  // read() -> get client name 
  will_return(__wrap_read, expected_client.conn.rx);
  will_return(__wrap_read, expected_client.name);
  will_return(__wrap_read, NAME_LEN);
  will_return(__wrap_read, NAME_LEN);

  // Connect second time //
  // accept() -> new client 
  will_return(__wrap_accept, accept_sock);
  will_return(__wrap_accept, NULL);
  will_return(__wrap_accept, NULL);
  will_return(__wrap_accept, expected_client.conn.tx);

  // read() -> get client name 
  will_return(__wrap_read, expected_client.conn.tx);
  will_return(__wrap_read, expected_client.name);
  will_return(__wrap_read, NAME_LEN);
  will_return(__wrap_read, NAME_LEN);

  // pthread_create() -> start routing client 
  will_return(__wrap_pthread_create, NULL);
  will_return(__wrap_pthread_create, sizeof(NULL));
  will_return(__wrap_pthread_create, 0);

  // pthread_detach() -> detach thread 
  will_return(__wrap_pthread_detach, 0);

  // Run start_accepting
  start_accepting((int *) 1);

  // Make sure client was correctly registered
  assert_int_equal(clients[0].conn.rx, expected_client.conn.rx);
  assert_int_equal(clients[0].conn.tx, expected_client.conn.tx);
  assert_memory_equal(clients[0].name, expected_client.name, NAME_LEN);

  // Make sure no other placeholders were affected 
  const client_t vacant_client = client_t_new();
  for(int x = 1; x < MAX_NUM_CLI; x++) 
  {
    assert_int_equal(clients[x].conn.rx, vacant_client.conn.rx);
    assert_int_equal(clients[x].conn.tx, vacant_client.conn.tx);
    assert_memory_equal(clients[x].name, vacant_client.name, NAME_LEN);
  }
}

void test_ipcd_start_routing_client()
{
  // Create placeholders 
  const char * msg = "trx send image.jpg";
  const size_t msg_len = 19;

  const char * expected_msg = "pwr send image.jpg";
  const size_t expected_msg_len = msg_len;

  // Add destination client to list of clients 
  const client_t dest_client = {
    .conn = {
      .rx = 32,
      .tx = 33,
    },
    .name = "trx"
  };
  clients[1] = dest_client;

  // Configure wrappers //
  // read() -> waiting for request from client 
  will_return(__wrap_read, expected_client.conn.tx);
  will_return(__wrap_read, msg);
  will_return(__wrap_read, msg_len);
  will_return(__wrap_read, msg_len);

  // write() -> send message to destination client 
  will_return(__wrap_write, dest_client.conn.rx);
  will_return(__wrap_write, expected_msg);
  will_return(__wrap_write, expected_msg_len);
  will_return(__wrap_write, expected_msg_len);

  // Run function 
  start_routing_client(&clients[0]);
}