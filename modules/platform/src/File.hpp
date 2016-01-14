#ifndef REDSTRAIN_MOD_PLATFORM_FILE_HPP
#define REDSTRAIN_MOD_PLATFORM_FILE_HPP

#include <string>
#include <redstrain/util/types.hpp>

#include "api.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API File {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef int Handle;
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef HANDLE Handle;
		typedef DWORD ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

		enum Direction {
			INPUT,
			OUTPUT,
			RANDOM_ACCESS
		};

		enum SeekWhence {
			OFFSET_FROM_START,
			OFFSET_FROM_END,
			OFFSET_FROM_HERE
		};

		enum TruncateMode {
			NO_TRUNCATE         = 00,
			TRUNCATE_IF_WRONLY  = 01,
			TRUNCATE_IF_RDWR    = 02,
			TRUNCATE_IF_WRITING = 03,
			TRUNCATE_IF_RDONLY  = 04,
			ALWAYS_TRUNCATE     = 07
		};

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		static const Handle INVALID_HANDLE = -1;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const Handle INVALID_HANDLE;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */

	  private:
		Handle handle;
		Direction dir;

	  public:
		File(const std::string&, Direction, bool = false, TruncateMode = TRUNCATE_IF_WRONLY);
		File(Handle, Direction);
		File(const File&);

		inline Handle getHandle() const {
			return handle;
		}

		util::MemorySize read(char*, util::MemorySize);
		void write(const char*, util::MemorySize);
		void close();
		void seek(util::FileOffset, SeekWhence);
		util::FileSize tell() const;
		void open(const std::string&, Direction, bool = false, TruncateMode = TRUNCATE_IF_WRONLY);

		void _uncheckedClose();

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILE_HPP */
