// Required libraries for testing
#include <check.h>
#include <stdlib.h>

START_TEST(first_test) { ck_assert_int_eq(0, 0); }
END_TEST

int main() {
  // Create suite and suite runner
  Suite *s = suite_create("first");
  SRunner *sr = srunner_create(s);

  // Create test case
  TCase *tc_core = tcase_create("core");

  // Add tests to test case
  tcase_add_test(tc_core, first_test);

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