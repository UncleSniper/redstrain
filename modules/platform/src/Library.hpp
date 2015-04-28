#ifndef REDSTRAIN_MOD_PLATFORM_LIBRARY_HPP
#define REDSTRAIN_MOD_PLATFORM_LIBRARY_HPP

#include <string>

#include "api.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Library {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef void* Handle;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef HMODULE Handle;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		static const Handle INVALID_HANDLE;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const Handle INVALID_HANDLE;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */
		static const char *const FILENAME_PREFIX;
		static const char *const FILENAME_SUFFIX;

	  private:
		Handle handle;

	  public:
		Library(const std::string&);
		Library(Handle handle) : handle(handle) {}
		Library(const Library& lib) : handle(lib.handle) {}

		void* resolve(const std::string&) const;
		void close();

		static void loadGlobal(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LIBRARY_HPP */
