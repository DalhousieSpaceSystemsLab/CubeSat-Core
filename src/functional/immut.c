#include <stdio.h>

#define immut(type) const type * const
#define val(name) *(name)

int main()
{
  int age_ = 19;
  immut(int) age = &age_;

  printf("%d is age\n", val(age));

  return 0;
}