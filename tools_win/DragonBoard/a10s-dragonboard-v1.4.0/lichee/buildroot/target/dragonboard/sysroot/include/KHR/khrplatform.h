/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009-2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef _MALI_KHRPLATFORM_H_
#define _MALI_KHRPLATFORM_H_

#ifndef KHRONOS_APICALL
#if (defined(_WIN32) || defined(__VC32__)) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__) /* Win32 and WinCE */
#   if defined (_DLL_EXPORTS)
#       define KHRONOS_APICALL __declspec(dllexport)
#   else
#       define KHRONOS_APICALL __declspec(dllimport)
#   endif
#elif defined (__SYMBIAN32__)
#   define KHRONOS_APICALL IMPORT_C
#else
#   define KHRONOS_APICALL
#endif
#endif

#if (defined(_WIN32) || defined(__VC32__)) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__) && !defined(_WIN32_WCE) /* Win32 but not WinCE */
#define KHRONOS_APIENTRY __stdcall
#else
#define KHRONOS_APIENTRY
#endif

typedef signed char   khronos_int8_t;
typedef unsigned char khronos_uint8_t;
typedef int           khronos_int32_t;
typedef float         khronos_float_t;
typedef long long khronos_int64_t;
typedef unsigned long long  khronos_uint64_t;

typedef int           khronos_intptr_t;
typedef int           khronos_ssize_t;

#define KHRONOS_SUPPORT_INT64   1

#ifdef _MSC_VER
typedef unsigned __int64    khronos_utime_nanoseconds_t;
typedef signed __int64      khronos_stime_nanoseconds_t;
#else
typedef unsigned long long  khronos_utime_nanoseconds_t;
typedef signed long long    khronos_stime_nanoseconds_t;
#endif

#endif /* _MALI_KHRPLATFORM_H_ */
