// Required libraries for testing
#include <check.h>
#include <stdlib.h>

// Headers for testing
#include "client_t.h"
#include "conn_t.h"

START_TEST(test_conn_t_new) {
  conn_t conn = conn_t_new();

  ck_assert_int_eq(conn.rx, -1);
  ck_assert_int_eq(conn.tx, -1);
}
END_TEST

START_TEST(test_conn_t_stat) {
  conn_t succ = {.rx = 25, .tx = 25};
  conn_t fail = conn_t_new();

  ck_assert_int_eq(conn_t_stat(succ), 0);
  ck_assert_int_eq(conn_t_stat(fail), -1);
}

START_TEST(test_conn_t_close) {
  conn_t conn = {.rx = 25, .tx = 25};
  conn_t_close(&conn);
  ck_assert_int_eq(conn.rx, -1);
  ck_assert_int_eq(conn.tx, -1);
}

int main() {
  // Create suite and suite runner
  Suite *s = suite_create("suite_conn_t");
  SRunner *sr = srunner_create(s);

  // Create test case
  TCase *tc_core = tcase_create("tcase_conn_t");

  // Add tests to test case
  tcase_add_test(tc_core, test_conn_t_new);
  tcase_add_test(tc_core, test_conn_t_stat);
  tcase_add_test(tc_core, test_conn_t_close);

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