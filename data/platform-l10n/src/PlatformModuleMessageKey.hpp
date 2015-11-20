#ifndef REDSTRAIN_DATA_PLATFORM_L10N_PLATFORMMODULEMESSAGEKEY_HPP
#define REDSTRAIN_DATA_PLATFORM_L10N_PLATFORMMODULEMESSAGEKEY_HPP

namespace redengine {
namespace platform {
namespace l10n {

	enum PlatformModuleMessageKey {
		MSG_UNIX_EPERM,
		MSG_UNIX_ENOENT,
		MSG_UNIX_ESRCH,
		MSG_UNIX_EINTR,
		MSG_UNIX_EIO,
		MSG_UNIX_ENXIO,
		MSG_UNIX_E2BIG,
		MSG_UNIX_ENOEXEC,
		MSG_UNIX_EBADF,
		MSG_UNIX_ECHILD,
		MSG_UNIX_EAGAIN,
		MSG_UNIX_ENOMEM,
		MSG_UNIX_EACCES,
		MSG_UNIX_EFAULT,
		MSG_UNIX_ENOTBLK,
		MSG_UNIX_EBUSY,
		MSG_UNIX_EEXIST,
		MSG_UNIX_EXDEV,
		MSG_UNIX_ENODEV,
		MSG_UNIX_ENOTDIR,
		MSG_UNIX_EISDIR,
		MSG_UNIX_EINVAL,
		MSG_UNIX_ENFILE,
		MSG_UNIX_EMFILE,
		MSG_UNIX_ENOTTY,
		MSG_UNIX_ETXTBSY,
		MSG_UNIX_EFBIG,
		MSG_UNIX_ENOSPC,
		MSG_UNIX_ESPIPE,
		MSG_UNIX_EROFS,
		MSG_UNIX_EMLINK,
		MSG_UNIX_EPIPE,
		MSG_UNIX_EDOM,
		MSG_UNIX_ERANGE,
		MSG_UNIX_EDEADLK,
		MSG_UNIX_ENAMETOOLONG,
		MSG_UNIX_ENOLCK,
		MSG_UNIX_ENOSYS,
		MSG_UNIX_ENOTEMPTY,
		MSG_UNIX_ELOOP,
		MSG_UNIX_ENOMSG,
		MSG_UNIX_EIDRM,
		MSG_UNIX_ECHRNG,
		MSG_UNIX_EL2NSYNC,
		MSG_UNIX_EL3HLT,
		MSG_UNIX_EL3RST,
		MSG_UNIX_ELNRNG,
		MSG_UNIX_EUNATCH,
		MSG_UNIX_ENOCSI,
		MSG_UNIX_EL2HLT,
		MSG_UNIX_EBADE,
		MSG_UNIX_EBADR,
		MSG_UNIX_EXFULL,
		MSG_UNIX_ENOANO,
		MSG_UNIX_EBADRQC,
		MSG_UNIX_EBADSLT,
		MSG_UNIX_EBFONT,
		MSG_UNIX_ENOSTR,
		MSG_UNIX_ENODATA,
		MSG_UNIX_ETIME,
		MSG_UNIX_ENOSR,
		MSG_UNIX_ENONET,
		MSG_UNIX_ENOPKG,
		MSG_UNIX_EREMOTE,
		MSG_UNIX_ENOLINK,
		MSG_UNIX_EADV,
		MSG_UNIX_ESRMNT,
		MSG_UNIX_ECOMM,
		MSG_UNIX_EPROTO,
		MSG_UNIX_EMULTIHOP,
		MSG_UNIX_EDOTDOT,
		MSG_UNIX_EBADMSG,
		MSG_UNIX_EOVERFLOW,
		MSG_UNIX_ENOTUNIQ,
		MSG_UNIX_EBADFD,
		MSG_UNIX_EREMCHG,
		MSG_UNIX_ELIBACC,
		MSG_UNIX_ELIBBAD,
		MSG_UNIX_ELIBSCN,
		MSG_UNIX_ELIBMAX,
		MSG_UNIX_ELIBEXEC,
		MSG_UNIX_EILSEQ,
		MSG_UNIX_ERESTART,
		MSG_UNIX_ESTRPIPE,
		MSG_UNIX_EUSERS,
		MSG_UNIX_ENOTSOCK,
		MSG_UNIX_EDESTADDRREQ,
		MSG_UNIX_EMSGSIZE,
		MSG_UNIX_EPROTOTYPE,
		MSG_UNIX_ENOPROTOOPT,
		MSG_UNIX_EPROTONOSUPPORT,
		MSG_UNIX_ESOCKTNOSUPPORT,
		MSG_UNIX_EOPNOTSUPP,
		MSG_UNIX_EPFNOSUPPORT,
		MSG_UNIX_EAFNOSUPPORT,
		MSG_UNIX_EADDRINUSE,
		MSG_UNIX_EADDRNOTAVAIL,
		MSG_UNIX_ENETDOWN,
		MSG_UNIX_ENETUNREACH,
		MSG_UNIX_ENETRESET,
		MSG_UNIX_ECONNABORTED,
		MSG_UNIX_ECONNRESET,
		MSG_UNIX_ENOBUFS,
		MSG_UNIX_EISCONN,
		MSG_UNIX_ENOTCONN,
		MSG_UNIX_ESHUTDOWN,
		MSG_UNIX_ETOOMANYREFS,
		MSG_UNIX_ETIMEDOUT,
		MSG_UNIX_ECONNREFUSED,
		MSG_UNIX_EHOSTDOWN,
		MSG_UNIX_EHOSTUNREACH,
		MSG_UNIX_EALREADY,
		MSG_UNIX_EINPROGRESS,
		MSG_UNIX_ESTALE,
		MSG_UNIX_EUCLEAN,
		MSG_UNIX_ENOTNAM,
		MSG_UNIX_ENAVAIL,
		MSG_UNIX_EISNAM,
		MSG_UNIX_EREMOTEIO,
		MSG_UNIX_EDQUOT,
		MSG_UNIX_ENOMEDIUM,
		MSG_UNIX_EMEDIUMTYPE,
		MSG_UNKNOWN_OS_ERROR,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR_CREATE,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR_DESTROY,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR_WAIT,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR_SIGNAL,
		MSG_CONDITION_VARIABLE_OPERATION_ERROR_UNKNOWN_OPERATION,
		MSG_CONSOLE_ERROR,
		MSG_CONSOLE_ERROR_CLEAR,
		MSG_CONSOLE_ERROR_SET_CURSOR_POSITION,
		MSG_CONSOLE_ERROR_SET_TEXT_ATTRIBUTE,
		MSG_CONSOLE_ERROR_UNKNOWN_OPERATION,
		MSG_SOCKET_IO_ERROR_DIRECTION,
		MSG_SOCKET_IO_ERROR_SEND,
		MSG_SOCKET_IO_ERROR_RECEIVE,
		MSG_SOCKET_IO_ERROR_UNKNOWN_DIRECTION,
		MSG_FILE_ERROR_ACCESS_INPUT,
		MSG_FILE_ERROR_ACCESS_OUTPUT,
		MSG_FILE_ERROR_ACCESS_RANDOM_ACCESS,
		MSG_FILE_ERROR_ACCESS_UNKNOWN_DIRECTION,
		MSG_FILE_ERROR_IO_INPUT,
		MSG_FILE_ERROR_IO_OUTPUT,
		MSG_FILE_ERROR_IO_RANDOM_ACCESS,
		MSG_FILE_ERROR_IO_UNKNOWN_DIRECTION,
		MSG_DATAGRAM_SOCKET_BIND_ERROR,
		MSG_DATAGRAM_SOCKET_CLOSE_ERROR,
		MSG_DATAGRAM_SOCKET_CREATE_ERROR,
		MSG_DATAGRAM_SOCKET_IO_ERROR,
		MSG_FILE_ACCESS_DENIED_ERROR,
		MSG_FILE_CLOSE_ERROR,
		MSG_FILE_IO_ERROR,
		MSG_FILE_OPEN_ERROR,
		MSG_FILE_SEEK_ERROR,
		MSG_FILESYSTEM_ERROR,
		MSG_FILE_TELL_ERROR,
		MSG__LAST
	};

}}}

#endif /* REDSTRAIN_DATA_PLATFORM_L10N_PLATFORMMODULEMESSAGEKEY_HPP */
