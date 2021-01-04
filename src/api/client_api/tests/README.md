Source modules that are placed in this (or sub-directories) Will be added
to the test tree. Tests are not normally built unless the current
cmake project is also the top-level cmake project. 

The CLI definition for BUILD_TESTING=ON can be provided to 
build tests when this project is included in a larger cmake project.

These source modules should be a single file executable translation unit
whose mainline returns 0 on success and a non-zero code on failure.

An example of such a template is provided in the code snippet below:

```C
/**
 * @file example_lib_test.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Example test file
 * @version 0.1
 * @date 2020-12-21
 *
 * @copyright Copyright (c) 2020 Carl Mattatall
 *
 * @note
 */
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "example_lib_public_header.h"


int main(void)
{   
    char         msg[] = "HelloWorld!!\n";
    unsigned int bytes_out;
    bytes_out = example_printf_wrapper(msg, sizeof(msg));
    assert(bytes_out == sizeof(msg));
    return 0;
}
```