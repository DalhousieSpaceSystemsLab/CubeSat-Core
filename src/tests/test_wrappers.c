/*
* tests_wrappers.c
*
*   purpose: Provide function wrappers for unit testing on functions with side effects.
*   author: alex amellal
*
*/

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
  
  // Create placeholders for mock data and mock data length
  char * mock_buf = (char *) mock(); 
  size_t mock_buflen = (size_t) mock();
  
  assert_true(mock_buflen <= buflen);
  
  // Copy mock data into buffer 
  memcpy(buf, mock_buf, mock_buflen);

  return mock();
}

// Wrapper for close function 
int __wrap_close(int sockfd)
{
  assert_int_equal(sockfd, mock());

  return mock();
}

// Wrapper for unlink function 
int __wrap_unlink(const char path[])
{
  assert_string_equal(path, mock());

  return (int) mock();
}

// Wrapper for bind function 
int __wrap_bind(int sockfd, struct sockaddr * address, socklen_t address_len)
{
  assert_int_equal(sockfd, mock());
  assert_memory_equal(address, mock(), sizeof(struct sockaddr));
  assert_int_equal(address_len, mock());

  return mock();
}

// Wrapper for listen function 
int __wrap_listen(int sockfd, int n)
{
  assert_int_equal(sockfd, mock());
  assert_int_equal(n, mock());

  return mock();
}

// Wrapper for accept function 
int __wrap_accept(int sockfd, struct sockaddr * address, socklen_t * address_len)
{
  assert_int_equal(sockfd, mock());
  assert_ptr_equal(address, mock());
  assert_ptr_equal(address_len, mock());
  
  return mock();
}

// Wrapper for pthread_create function 
int __wrap_pthread_create(pthread_t * id, const pthread_attr_t *attr, void *(*start_routine) (void *), void * arg)
{
  assert_non_null(id);
  assert_non_null(start_routine);
  assert_memory_equal(arg, mock(), mock());

  return mock();
}

// Wrapper for pthread_detach function 
int __wrap_pthread_detach(pthread_t id)
{
  return mock();
}