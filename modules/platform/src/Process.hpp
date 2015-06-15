#ifndef REDSTRAIN_MOD_PLATFORM_PROCESS_HPP
#define REDSTRAIN_MOD_PLATFORM_PROCESS_HPP

#include <map>

#include "File.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Process {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef pid_t Handle;
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef DWORD Handle;
		typedef DWORD ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_DOXYGEN
		/**
		 * Platform process identifier. An instance of this type
		 * indicates which actual process an instance of Process
		 * represents. On all modern platforms, this is a
		 * primitive integral type, but clients should treat
		 * it as opaque in the interest of portability. As
		 * such, the only operations guaranteed to be supported
		 * by this type are copy-construction (including
		 * pass-by-value) and destruction.
		 *
		 * @see getHandle()
		 * @since 0.1.0
		 */
		typedef PlatformSpecific Handle;
		/**
		 * Process operation error code. If a process control
		 * operation fails at the platform level, the platform
		 * will supply an error code indicating what prevented
		 * the operation from succeeding. Such an error code
		 * will be exposed to the client by the exception thrown
		 * in such a case. On all modern platforms, this is a
		 * primitive integral type, but clients should treat
		 * it as opaque in the interest of portability. As
		 * such, the only operations guaranteed to be supported
		 * by this type are copy-construction (including
		 * pass-by-value), destruction and mapping an error
		 * code to a human-readable error message via
		 * @link getErrorMessage(ErrorCode) getErrorMessage@endlink.
		 *
		 * @see getErrorMessage(ErrorCode), ProcessOperationError
		 * @since 0.1.0
		 */
		typedef PlatformSpecific ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  protected:
		Handle handle;
		File::Handle stdinHandle, stdoutHandle, stderrHandle;

	  public:
		Process(Handle);
		Process(Handle, File::Handle, File::Handle, File::Handle);
		Process(const Process&);

		inline Handle getHandle() const {
			return handle;
		}

		inline File::Handle getStandardInputHandle() const {
			return stdinHandle;
		}

		inline File::Handle getStandardOutputHandle() const {
			return stdoutHandle;
		}

		inline File::Handle getStandardErrorHandle() const {
			return stderrHandle;
		}

		void terminate();
		void kill();
		void interrupt();
		int wait();

		static std::string getErrorMessage(ErrorCode);
		static const char* getenv(const std::string&);
		static void putenv(const std::string&, const std::string&);
		static void getenv(std::map<std::string, std::string>&);
		static std::string resolveExecutable(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROCESS_HPP */
