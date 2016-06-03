#ifndef REDSTRAIN_MOD_REDMOND_ENVIRONMENT_HPP
#define REDSTRAIN_MOD_REDMOND_ENVIRONMENT_HPP

// ==== Architectures ====

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define REDSTRAIN_ENV_ARCH_AMD64
#elif defined(_M_X64) || defined(_M_AMD64)
#define REDSTRAIN_ENV_ARCH_AMD64
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
#define REDSTRAIN_ENV_ARCH_I686
#endif /* architectures */

// ==== Compilers ====

#if defined(__GNUC__)

// GCC
#define REDSTRAIN_ENV_COMPILER_GNU
#define REDSTRAIN_ENV_COMPILER_MAJOR __GNUC__
#define REDSTRAIN_ENV_COMPILER_MINOR __GNUC_MINOR__
#ifdef __GNUC_PATCHLEVEL__
#define REDSTRAIN_ENV_COMPILER_REVISION __GNUC_PATCHLEVEL__
#else /* !__GNUC_PATCHLEVEL__ */
#define REDSTRAIN_ENV_COMPILER_REVISION 0
#endif /* __GNUC_PATCHLEVEL__ */

#elif defined(_MSC_VER)

// MSVC
//   Sweet jumping Lyssa, the version info stuff
//   for the Microsoft compiler is a mess!
#define REDSTRAIN_ENV_COMPILER_MSVC
#define REDSTRAIN_ENV_COMPILER_MAJOR (_MSC_VER < 900 ? 1 : _MSC_VER / 100 - 6)
#define REDSTRAIN_ENV_COMPILER_MINOR (_MSC_VER % 100 / 10)
#ifdef _MSC_FULL_VER
#define REDSTRAIN_ENV_COMPILER_REVISION (_MSC_FULL_VER % (_MSC_VER < 1400 ? 10000 : 100000))
#else /* !_MSC_FULL_VER */
#define REDSTRAIN_ENV_COMPILER_REVISION 0
#endif /* _MSC_FULL_VER */

#endif /* compilers */

// ==== Operating Systems ====

#if defined(__gnu_linux__)
#define REDSTRAIN_ENV_OS_LINUX
#elif defined(__linux__) && !defined(__ANDROID__)
#define REDSTRAIN_ENV_OS_LINUX
#elif defined(_WIN32)
#define REDSTRAIN_ENV_OS_WINDOWS
#endif /* OSes */

#endif /* REDSTRAIN_MOD_REDMOND_ENVIRONMENT_HPP */
