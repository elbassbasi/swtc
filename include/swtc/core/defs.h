/*
 * Name:        defs.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CORE_DEFS_H_
#define SWTC_CORE_DEFS_H_
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef intptr_t wintptr;
typedef unsigned char wuchar;
typedef wchar_t wchar;
typedef wchar_t wwchar;
typedef unsigned int wunichar;
typedef short wshort;
typedef unsigned short wushort;
typedef int wint;
typedef unsigned int wuint;
typedef long long int wint64;
typedef unsigned long long int wuint64;
typedef wintptr wresult;
/*
 * DLL_EXPORT
 */
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    #ifdef __GNUC__
      #define DLL_EXPORT __attribute__ ((dllexport))
    #else
      #define DLL_EXPORT __declspec(dllexport)
    #endif
    #ifdef __GNUC__
      #define DLL_IMPORT __attribute__ ((dllimport))
    #else
      #define DLL_IMPORT __declspec(dllimport)
    #endif
#else
#if __GNUC__ >= 4
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT
#else
    #define DLL_IMPORT
    #define DLL_IMPORT
  #endif
#endif

#ifdef SWTBUILD
#define SWT_PUBLIC DLL_EXPORT
#else
//#define SWT_PUBLIC DLL_IMPORT
#define SWT_PUBLIC
#endif
/*
 * MIN and MAX
 */
#define WMIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define WMAX(X, Y) (((X) > (Y)) ? (X) : (Y))
/*
 * ctz and clz
 */
#ifdef __GNUC__
#define clz32(result,value) result = __builtin_clz(value)
#define ctz32(result,value) result = __builtin_ctz(value)
#define clz64(result,value) result = __builtin_clz(value)
#define ctz64(result,value) result = __builtin_ctz(value)
#elif _MSC_VER
#define clz32(result,value) _BitScanReverse(&result,value)
#define ctz32(result,value) _BitScanForward(&result,value)
#define clz64(result,value) _BitScanReverse64(&result,value)
#define ctz64(result,value) _BitScanForward64(&result,value)
#else
#endif
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CORE_DEFS_H_ */
