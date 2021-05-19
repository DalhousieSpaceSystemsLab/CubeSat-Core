// Required libraries for testing
#include <check.h>
#include <stdlib.h>

// Test headers
#include "client_t.h"

START_TEST(test_client_t_new) {
  client_t client = client_t_new();
  conn_t conn = conn_t_new();
  ck_assert_int_eq(client.conn.rx, conn.rx);
  ck_assert_int_eq(client.conn.tx, conn.tx);

  char blank_name[NAME_LEN];
  memset(blank_name, 0, NAME_LEN);

  ck_assert_mem_eq(client.name, blank_name, NAME_LEN);
}
END_TEST

START_TEST(test_client_t_stat) {
  client_t client_used = client_t_new();
  client_t client_unused = client_t_new();

  char name[NAME_LEN];
  memset(name, 'a', NAME_LEN);
  strncpy(client_used.name, name, NAME_LEN);

  client_used.conn.rx = 25;
  client_unused.conn.rx = 25;
  client_used.conn.tx = 25;
  client_unused.conn.tx = 25;

  ck_assert_int_eq(client_t_stat(client_used), 1);
  ck_assert_int_eq(client_t_stat(client_unused), 0);
}
END_TEST

START_TEST(test_client_t_close) {
  client_t client = client_t_new();
  client_t blank_client = client_t_new();
  memset(client.name, 'a', NAME_LEN);
  client.conn.rx = 25;
  client.conn.tx = 25;

  client_t_close(&client);

  ck_assert_mem_eq(client.name, blank_client.name, NAME_LEN);
  ck_assert_int_eq(client.conn.rx, blank_client.conn.rx);
  ck_assert_int_eq(client.conn.tx, blank_client.conn.tx);
}
END_TEST

int main() {
  // Create suite and suite runner
  Suite *s = suite_create("suite_client_t");
  SRunner *sr = srunner_create(s);

  // Create test case
  TCase *tc_core = tcase_create("core");

  // Add tests to test case
  tcase_add_test(tc_core, test_client_t_new);
  tcase_add_test(tc_core, test_client_t_stat);
  tcase_add_test(tc_core, test_client_t_close);

  // Add test case to suite
  suite_add_tcase(s, tc_core);

  // Run suite runner
  srunner_run_all(sr, CK_VERBOSE);

  // Capture number of failed tests
  int n_failed = srunner_ntests_failed(sr);

  // Free suite runner
  srunner_free(sr);

  // done
  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}