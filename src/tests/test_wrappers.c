#include "ipc/tests.h"

// Wrapper for socket function
int __wrap_socket(int domain, int type, int protocol)
{
  assert_int_equal(domain, (int) mock());
  assert_int_equal(type, (int) mock());
  assert_int_equal(protocol, (int) mock());

  return (int) mock();
}

// Wrapper for connect function 
int __wrap_connect(int sock, struct sockaddr * address, socklen_t address_len)
{
  assert_int_equal(sock, mock());
  assert_non_null(address);
  assert_memory_equal(address, mock(), sizeof(struct sockaddr));
  assert_int_equal(address_len, mock());

  return mock();
}

// Wrapper for write function
int __wrap_write(int sock, void * buf, size_t buflen)
{
  assert_int_equal(sock, mock());
  assert_non_null(buf);
  assert_memory_equal(buf, mock(), buflen);
  assert_int_equal(buflen, mock());

  return mock();
}

// Wrapper for read function
int __wrap_read(int sock, void * buf, size_t buflen)
{
  assert_int_equal(sock, mock());
  assert_non_null(buf);
  memcpy(buf, (void *) mock(), mock());
  assert_int_equal(buflen, mock());

  return mock();
}