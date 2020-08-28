/*
* tests.c
*
*   purpose: Unit testing using cmocka library for core software.
*   author: alex amellal
*
*/

#include "ipc/tests.h"

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_client_api_connect),
      cmocka_unit_test(test_client_api_send),
      cmocka_unit_test(test_client_api_recv),
      cmocka_unit_test(test_client_api_refresh),
      cmocka_unit_test(test_client_api_disconnect),
      cmocka_unit_test(test_ipcd_init),
      cmocka_unit_test(test_ipcd_close),
      cmocka_unit_test(test_ipcd_start_accepting),
      cmocka_unit_test(test_ipcd_start_routing_client),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}