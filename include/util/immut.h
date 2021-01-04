/*
* immut.h
*
*   purpose: provides a framework for runtime constants in C.
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_UTIL_IMMUT_H
#define CUBESAT_CORE_INCLUDE_UTIL_IMMUT_H

#define immut(type) const type * 
#define val(name) *(name)

#endif // end of header guard for CUBESAT_CORE_INCLUDE_UTIL_IMMUT_H