#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKET_HPP

#include "api.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

#include "StreamSocket.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocket : public StreamSocket {

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
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		std::string pipeName;
#endif /* OS-specific data members */

	  private:
		Handle acceptHandle();

		static std::string canonizalizeSocketName(const std::string&);

	  public:
		LocalSocket();
		LocalSocket(Handle);
		LocalSocket(const LocalSocket&);

		inline Handle getHandle() const {
			return handle;
		}

		void listen(const std::string&, unsigned = 16u);
		void connect(const std::string&);
		LocalSocket accept();

		virtual bool isOpen() const;
		virtual size_t read(char*, size_t);
		virtual void write(const char*, size_t);
		virtual void close();
		virtual void _uncheckedClose();
		virtual StreamSocket* acceptAgnostic();

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKET_HPP */
