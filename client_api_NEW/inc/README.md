Header files that are placed in this (or sub directories) will be added as 
public include directories of the current target.

When other targets link against the current target with PUBLIC scope,
they gain access to the headers indexed from this directory and below.

An example of such a header file is provided in the snippet below:


```C
#ifndef __EXAMPLE_LIB_PUBLIC_HEADER_H__
#define __EXAMPLE_LIB_PUBLIC_HEADER_H__
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */

int example_printf_wrapper(char *buf, unsigned int buflen);

#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* __EXAMPLE_LIB_PUBLIC_HEADER_H__ */
```