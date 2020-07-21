// IPC Client API 
#include "ipc/settings.h"
#include "ipc/client_api.h"

// Standard C libraries
#include <stdio.h>

// Unit testing library
#include <check.h>

// Wrapper for socket function
int __wrap_socket(int domain, int type, int protocol)
{
  return 11;
}

// Wrapper for write function
int __wrap_write(int sock, void * buf, size_t buflen)
{
  return buflen;
}

// Wrapper for read function
int __wrap_read(int sock, void * buf, size_t buflen)
{
  // Create placeholder for fake message 
  char * msg = "alx do thing please\0";

  // Copy fake message into output buffer
  strcpy(buf, msg);

  // done 
  return strlen(msg);
}

START_TEST (test_client_api_connect) {
  ck_assert_int_eq(ipc_connect("rrr"), 0);
} END_TEST

int main() {
  Suite *s;
  TCase *tc;

  s = suite_create("loris core software");
  tc = tcase_create("client api");

  tcase_add_test(tc, test_client_api_connect);
  suite_add_tcase(s, tc);

  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  int no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (no_failed == 0) ? 0 : -1;
}