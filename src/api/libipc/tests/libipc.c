#include <check.h>
#include <stdlib.h>

START_TEST(first_test) { ck_assert_int_eq(1, 1); }
END_TEST

int main() {
  // Create suite and suite runner
  Suite *s = suite_create("first");
  SRunner *sr = srunner_create(s);

  // Run suite runner
  srunner_run_all(sr, CK_NORMAL);

  // Capture number of failed tests
  int n_failed = srunner_ntests_failed(sr);

  // Free suite runner
  srunner_free(sr);

  // done
  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}