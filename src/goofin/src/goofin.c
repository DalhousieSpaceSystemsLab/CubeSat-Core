/**
 * @file goofin.c
 * @author Alex Amellal (loris@alexamellal.com)
 * @brief Coding sandbox for this repo; just for goofin & testing
 * @version 0.1
 * @date 2021-05-28
 *
 * @copyright Dalhousie Space Systems Lab (c) 2021
 *
 */

#include <errno.h>
#include <stdio.h>

#define PART_THREE(x) \
  if (x > 0) printf("my_num = %d\n", my_num)
#define PART_TWO(x) PART_THREE(x)
#define PART_ONE(x) PART_TWO(x)

int main() {
  int my_num = 10;
  PART_ONE(1);

  return 0;
}