/*
 * Name:        atomic.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_RUNTIME_ATOMIC_H_
#define SWT_RUNTIME_ATOMIC_H_
#include "../core/core.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef volatile wintptr watomic_intptr;
typedef volatile char watomic_char;
typedef volatile wuchar watomic_uchar;
typedef volatile short watomic_short;
typedef volatile wushort watomic_ushort;
typedef volatile int watomic_int;
typedef volatile wuint watomic_uint;
typedef volatile wint64 watomic_int64;
typedef volatile wuint64 watomic_uint64;

SWT_PUBLIC int watomic_int_store(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_load(watomic_int *ptr);
SWT_PUBLIC int watomic_int_exchange(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_compare_exchange_strong(watomic_int *ptr, int val,
		int des);
SWT_PUBLIC int watomic_int_compare_exchange_weak(watomic_int *ptr, int val,
		int des);
SWT_PUBLIC int watomic_int_fetch_add(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_fetch_sub(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_fetch_or(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_fetch_xor(watomic_int *ptr, int val);
SWT_PUBLIC int watomic_int_fetch_and(watomic_int *ptr, int val);
#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_ATOMIC_H_ */
