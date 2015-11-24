#include "catalog.hpp"
#include "oserrorl10n.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif /* OS-specific includes */

using redengine::text::String16;
using redengine::locale::Locale;

namespace redengine {
namespace platform {
namespace l10n {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

#if !defined(EAGAIN) && defined(EWOULDBLOCK)
#define EAGAIN EWOULDBLOCK
#endif

#if !defined(EDEADLK) && defined(EDEADLOCK)
#define EDEADLK EDEADLOCK
#endif

	REDSTRAIN_PLATFORM_L10N_API String16 localizeOSErrorMessage16(GeneralizedErrorCode error, const Locale& locale) {
		PlatformModuleMessageKey key;
		switch(static_cast<int>(error)) {
			#define clamp(constant) \
				case constant: \
					key = MSG_UNIX_ ## constant; \
					break;
			clamp(EPERM)
			clamp(ENOENT)
			clamp(ESRCH)
			clamp(EINTR)
			clamp(EIO)
			clamp(ENXIO)
			clamp(E2BIG)
			clamp(ENOEXEC)
			clamp(EBADF)
			clamp(ECHILD)
			clamp(EAGAIN)
			clamp(ENOMEM)
			clamp(EACCES)
			clamp(EFAULT)
			clamp(ENOTBLK)
			clamp(EBUSY)
			clamp(EEXIST)
			clamp(EXDEV)
			clamp(ENODEV)
			clamp(ENOTDIR)
			clamp(EISDIR)
			clamp(EINVAL)
			clamp(ENFILE)
			clamp(EMFILE)
			clamp(ENOTTY)
			clamp(ETXTBSY)
			clamp(EFBIG)
			clamp(ENOSPC)
			clamp(ESPIPE)
			clamp(EROFS)
			clamp(EMLINK)
			clamp(EPIPE)
			clamp(EDOM)
			clamp(ERANGE)
			clamp(EDEADLK)
			clamp(ENAMETOOLONG)
			clamp(ENOLCK)
			clamp(ENOSYS)
			clamp(ENOTEMPTY)
			clamp(ELOOP)
			clamp(ENOMSG)
			clamp(EIDRM)
			clamp(ECHRNG)
			clamp(EL2NSYNC)
			clamp(EL3HLT)
			clamp(EL3RST)
			clamp(ELNRNG)
			clamp(EUNATCH)
			clamp(ENOCSI)
			clamp(EL2HLT)
			clamp(EBADE)
			clamp(EBADR)
			clamp(EXFULL)
			clamp(ENOANO)
			clamp(EBADRQC)
			clamp(EBADSLT)
			clamp(EBFONT)
			clamp(ENOSTR)
			clamp(ENODATA)
			clamp(ETIME)
			clamp(ENOSR)
			clamp(ENONET)
			clamp(ENOPKG)
			clamp(EREMOTE)
			clamp(ENOLINK)
			clamp(EADV)
			clamp(ESRMNT)
			clamp(ECOMM)
			clamp(EPROTO)
			clamp(EMULTIHOP)
			clamp(EDOTDOT)
			clamp(EBADMSG)
			clamp(EOVERFLOW)
			clamp(ENOTUNIQ)
			clamp(EBADFD)
			clamp(EREMCHG)
			clamp(ELIBACC)
			clamp(ELIBBAD)
			clamp(ELIBSCN)
			clamp(ELIBMAX)
			clamp(ELIBEXEC)
			clamp(EILSEQ)
			clamp(ERESTART)
			clamp(ESTRPIPE)
			clamp(EUSERS)
			clamp(ENOTSOCK)
			clamp(EDESTADDRREQ)
			clamp(EMSGSIZE)
			clamp(EPROTOTYPE)
			clamp(ENOPROTOOPT)
			clamp(EPROTONOSUPPORT)
			clamp(ESOCKTNOSUPPORT)
			clamp(EOPNOTSUPP)
			clamp(EPFNOSUPPORT)
			clamp(EAFNOSUPPORT)
			clamp(EADDRINUSE)
			clamp(EADDRNOTAVAIL)
			clamp(ENETDOWN)
			clamp(ENETUNREACH)
			clamp(ENETRESET)
			clamp(ECONNABORTED)
			clamp(ECONNRESET)
			clamp(ENOBUFS)
			clamp(EISCONN)
			clamp(ENOTCONN)
			clamp(ESHUTDOWN)
			clamp(ETOOMANYREFS)
			clamp(ETIMEDOUT)
			clamp(ECONNREFUSED)
			clamp(EHOSTDOWN)
			clamp(EHOSTUNREACH)
			clamp(EALREADY)
			clamp(EINPROGRESS)
			clamp(ESTALE)
			clamp(EUCLEAN)
			clamp(ENOTNAM)
			clamp(ENAVAIL)
			clamp(EISNAM)
			clamp(EREMOTEIO)
			clamp(EDQUOT)
			clamp(ENOMEDIUM)
			clamp(EMEDIUMTYPE)
			#undef clamp
			default:
				key = MSG_UNKNOWN_OS_ERROR;
				break;
		}
		PlatformModuleMessageCatalog16& catalog = getDefaultPlatformModuleMessageCatalog16();
		if(key == MSG_UNKNOWN_OS_ERROR)
			return catalog.formatMessage(locale, key, static_cast<int32_t>(error));
		else
			return catalog.formatMessage(locale, key);
	}

	REDSTRAIN_PLATFORM_L10N_API String16 localizeOSNetworkingErrorMessage16(GeneralizedErrorCode error,
			const Locale& locale) {
		PlatformModuleMessageKey key;
		switch(static_cast<int>(error)) {
			#define clamp(constant) \
				case constant: \
					key = MSG_UNIX_NET_ ## constant; \
					break;
			clamp(HOST_NOT_FOUND)
			clamp(NO_ADDRESS)
			clamp(NO_RECOVERY)
			clamp(TRY_AGAIN)
			#undef clamp
			default:
				key = MSG_UNKNOWN_OS_ERROR;
				break;
		}
		PlatformModuleMessageCatalog16& catalog = getDefaultPlatformModuleMessageCatalog16();
		if(key == MSG_UNKNOWN_OS_ERROR)
			return catalog.formatMessage(locale, key, static_cast<int32_t>(error));
		else
			return catalog.formatMessage(locale, key);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

#error TODO: support Windows

	REDSTRAIN_PLATFORM_L10N_API String16 localizeOSNetworkingErrorMessage16(GeneralizedErrorCode error,
			const Locale& locale) {
		return localizeOSErrorMessage16(error, locale);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

}}}
