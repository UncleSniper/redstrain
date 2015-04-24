#ifndef REDSTRAIN_MOD_PLATFORM_PLATFORM_HPP
#define REDSTRAIN_MOD_PLATFORM_PLATFORM_HPP

#define REDSTRAIN_PLATFORM_OS_UNIX 1
#define REDSTRAIN_PLATFORM_OS_WINDOWS 2
#define REDSTRAIN_PLATFORM_OS_DOXYGEN 3

#ifndef REDSTRAIN_PLATFORM_OS
#if defined(__unix__) || defined(__unix)
#define REDSTRAIN_PLATFORM_OS REDSTRAIN_PLATFORM_OS_UNIX
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define REDSTRAIN_PLATFORM_OS REDSTRAIN_PLATFORM_OS_WINDOWS
#else /* no known OS */
#error Operating system not supported
#endif /* os detection */
#endif /* !REDSTRAIN_PLATFORM_OS */

#if REDSTRAIN_PLATFORM_OS != REDSTRAIN_PLATFORM_OS_UNIX && REDSTRAIN_PLATFORM_OS != REDSTRAIN_PLATFORM_OS_WINDOWS
#error Unknown operating system specified
#endif /* unknown OS specified */

#define REDSTRAIN_PLATFORM_BIG_ENDIAN 1
#define REDSTRAIN_PLATFORM_LITTLE_ENDIAN 2

#ifndef REDSTRAIN_PLATFORM_ENDIANNESS
#ifdef __GLIBC__
#include <endian.h>
#else /* !__GLIBC__ */
#include <sys/param.h>
#endif /* !__GLIBC__ */
#ifdef __BYTE_ORDER
#if __BYTE_ORDER == __BIG_ENDIAN
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_BIG_ENDIAN
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#endif /* values of __BYTE_ORDER */
#endif /* __BYTE_ORDER */
#endif /* !REDSTRAIN_PLATFORM_ENDIANNESS */

#ifndef REDSTRAIN_PLATFORM_ENDIANNESS
#if defined(__BIG__ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__)
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_BIG_ENDIAN
#elif defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__)
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_BIG_ENDIAN
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__)
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#elif defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define REDSTRAIN_PLATFORM_ENDIANNESS REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#else /* no known endianness */
#error Failed to detect endianness, define REDSTRAIN_PLATFORM_ENDIANNESS manually
#endif /* endianness detection */
#endif /* !REDSTRAIN_PLATFORM_ENDIANNESS */

#if REDSTRAIN_PLATFORM_ENDIANNESS != REDSTRAIN_PLATFORM_BIG_ENDIAN && REDSTRAIN_PLATFORM_ENDIANNESS != REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#error Unknown endianness specified
#endif /* unknown endianness specified */

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#ifdef WINVER
#undef WINVER
#endif /* WINVER */
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif /* _WIN32_WINNT */
#ifdef REDSTRAIN_PLATFORM_WINXP_COMPAT
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#else /* Vista+ WinAPI */
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#endif /* select WinAPI version */
#endif /* platform defines for system headers */

#endif /* REDSTRAIN_MOD_PLATFORM_PLATFORM_HPP */
