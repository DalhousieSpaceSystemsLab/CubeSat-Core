#include "ipc/tests.h"

int main() 
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_client_api_connect),
    cmocka_unit_test(test_client_api_send),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}