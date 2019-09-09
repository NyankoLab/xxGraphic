#pragma once

#ifndef _HAS_EXCEPTIONS
#   define _HAS_EXCEPTIONS 0
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE__)
#   include <pthread.h>
#   include <sys/syscall.h>
#   include <sys/time.h>
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE
#       define xxIOS 1
#   elif TARGET_OS_OSX
#       define xxMACOS 1
#   endif
#   if TARGET_OS_MACCATALYST
#       define xxMACCATALYST 1
#   endif
#   include <unistd.h>
#endif

#if defined(_MSC_VER)
#   include <intrin.h>
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
#   define wglCreateContext wglCreateContext_unused
#   define wglDeleteContext wglDeleteContext_unused
#   define wglGetCurrentContext wglGetCurrentContext_unused
#   define wglGetCurrentDC wglGetCurrentDC_unused
#   define wglGetProcAddress wglGetProcAddress_unused
#   define wglMakeCurrent wglMakeCurrent_unused
#   define wglShareLists wglShareLists_unused
#   include <windows.h>
#   undef wglCreateContext
#   undef wglDeleteContext
#   undef wglGetCurrentContext
#   undef wglGetCurrentDC
#   undef wglGetProcAddress
#   undef wglMakeCurrent
#   undef wglShareLists
#   pragma warning(disable:4201)
#   define xxWINDOWS 1
#endif

#ifndef xxEXTERN
#   if defined(__cplusplus)
#       define xxEXTERN extern "C"
#   else
#       define xxEXTERN extern
#   endif
#endif

#ifndef xxAPI
#   if defined(_WINDLL)
#       define xxAPI xxEXTERN __declspec(dllexport)
#   elif defined(_MSC_VER)
#       define xxAPI xxEXTERN __declspec(dllimport)
#   else
#       define xxAPI xxEXTERN
#   endif
#endif

#if defined(__GNUC__)
#   define xxLikely(x)              __builtin_expect((x), 1)
#   define xxUnlikely(x)            __builtin_expect((x), 0)
#else
#   define xxLikely(x)              (x)
#   define xxUnlikely(x)            (x)
#endif

#define xxSizeOf(var)               (sizeof(var))
#define xxCountOf(var)              (sizeof(var) / sizeof(*var))
#define xxOffsetOf(st, m)           (offsetof(st, m))

#if defined(__APPLE__)
#   define xxAlloc(T, count)        (T*)malloc(sizeof(T) * count)
#   define xxRealloc(T, count, ptr) (T*)realloc(ptr, sizeof(T) * count)
#   define xxFree(ptr)              free(ptr)
#else
#   define xxAlloc(T, count)        (T*)_aligned_malloc(sizeof(T) * count, 16)
#   define xxRealloc(T, count, ptr) (T*)_aligned_realloc(ptr, sizeof(T) * count, 16)
#   define xxFree(ptr)              _aligned_free(ptr)
#endif

#define xxRotateLeft(v, s)          (v << s) | (v >> (sizeof(v) * 8 - s))
#define xxRotateRight(v, s)         (v >> s) | (v << (sizeof(v) * 8 - s))

#define xxLocalBreak()              switch (0) case 0:

//==============================================================================
//  TSC
//==============================================================================
xxAPI uint64_t xxTSC();
xxAPI float xxGetCurrentTime();
//==============================================================================
//  Process / Thread ID
//==============================================================================
xxAPI uint64_t xxGetCurrentProcessId();
xxAPI uint64_t xxGetCurrentThreadId();
xxAPI int xxGetIncrementThreadId();
//==============================================================================
//  Logger
//==============================================================================
xxAPI int xxLog(const char* tag, const char* format, ...);
//==============================================================================
//  MD5
//==============================================================================
xxAPI const uint8_t* xxMD5(const void* data, int len, uint8_t* digest);
xxAPI const uint8_t* xxDXBCChecksum(const void* data, int len, uint8_t* digest);
