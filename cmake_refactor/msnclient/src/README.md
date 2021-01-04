Source modules placed in this directory or subdirectories will be added
as private translation units to the current target.

Headers in this directory or nested subdirectories will be added as PRIVATE
include directories for the target. 

A possible directory project structure may look like this:

```
src
 \
  \ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
         |             |                       |
   module folder   private_TU.c        private header.h      
         |
         \ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
               \                \               \
                \                \               \
                 module_src1.c   module_src2.c   module_header.h
```

Some template snippests are also provided below:


Example private header template <br/>

```C
#ifndef __EXAMPLE_LIB_PRIVATE_HEADER_H__
#define __EXAMPLE_LIB_PRIVATE_HEADER_H__
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */

/**
 * @brief Get the ptr to tmp buf
 * @return char* ptr to tmp buf
 */
char *get_ptr_to_tmp_buf(void);


/**
 * @brief Get size of internal buffer
 * @return size of internal buffer
 */
unsigned int get_internal_buffer_size(void);

#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* __EXAMPLE_LIB_PRIVATE_HEADER_H__ */
```


Private source file example template <br/>

```C
/**
 * @file private_source_file.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief source module to demonstrate usage of cmake library project template
 * @version 0.1
 * @date 2020-12-21
 *
 * @copyright Copyright (c) 2020 carl mattatall
 */

static char tmp_buf_internal[2000];

char *get_ptr_to_tmp_buf(void)
{
    return tmp_buf_internal;
}


unsigned int get_internal_buffer_size(void)
{
    return sizeof(tmp_buf_internal);
}
```



Example library source module template: <br/>

```C
/**
 * @file example_lib.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Example source module to add to the example lib
 * @version 0.1
 * @date 2020-12-21
 *
 * @copyright Copyright (c) 2020 carl matttatall
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "example_lib_private_header.h"

int example_printf_wrapper(char *buf, unsigned int buflen)
{
    char *internal_buffer;
    internal_buffer = get_ptr_to_tmp_buf();

    unsigned int copy_len          = buflen;
    unsigned int internal_buf_size = get_internal_buffer_size();
    if (copy_len > internal_buf_size)
    {
        copy_len = internal_buf_size;
    }
    strncpy(internal_buffer, buf, copy_len);
    printf("%.*s", copy_len, internal_buffer);
    return copy_len;
}

```
