/*
 * atomic.h
 *
 *  Created on: 25 juil. 2021
 *      Author: azeddine
 */

#ifndef INCLUDE_SWTC_CORE_ATOMIC_H_
#define INCLUDE_SWTC_CORE_ATOMIC_H_
#include "defs.h"
#ifdef __GNUC__
#define atomic_cmpxchg32(ptr,oldval,newval) __sync_val_compare_and_swap(ptr,oldval,newval)
#define atomic_add32(ptr,value) __sync_fetch_and_add(ptr,value)
#define atomic_sub32(ptr,value) __sync_fetch_and_sub(ptr,value)
#define atomic_or32(ptr,value) __sync_fetch_and_or(ptr,value)
#define atomic_and32(ptr,value) __sync_fetch_and_and(ptr,value)
#define atomic_xor32(ptr,value) __sync_fetch_and_xor(ptr,value)
#elif _MSC_VER
#else
#endif



#endif /* INCLUDE_SWTC_CORE_ATOMIC_H_ */
