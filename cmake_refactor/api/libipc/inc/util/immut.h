/*
 * immut.h
 *
 *   purpose: provides a framework for runtime constants in C.
 *   author: alex amellal
 *
 */
#ifndef __IMMUT_H__
#define __IMMUT_H__
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */

#define immut(type) const type *
#define val(name) *(name)

#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* __IMMUT_H__ */
