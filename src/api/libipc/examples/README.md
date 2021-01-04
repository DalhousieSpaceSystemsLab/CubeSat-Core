Source modules that are placed in this (or sub-directories) Will be added
to the examples build tree. Examples are not normally built unless the current
cmake project is also the top-level cmake project. 

The CLI definition for BUILD_EXAMPLES=ON can be provided to 
build tests when this project is included in a larger cmake project.

Ideally, the examples should be a single file executable (with a mainline)
whose purpose is to demonstrate library usage.

An example of such a template is provided in the code snippet below:

```C
/**
 * @file example_of_library_usage.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Source module for template file to 
 * @version 0.1
 * @date 2020-12-29
 *
 * @copyright Copyright (c) 2020 Carl Mattatall
 *
 * @note
 */

#include "example_lib_public_header.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        int i;
        for (i = 1; i < argc; i++)
        {
            printf_wrapper("Hello World\n");
        }
    }
    else
    {
    }
    return 0;
}
```