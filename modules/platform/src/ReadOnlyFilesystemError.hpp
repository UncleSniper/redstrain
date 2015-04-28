#ifndef REDSTRAIN_MOD_PLATFORM_READONLYFILESYSTEMERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_READONLYFILESYSTEMERROR_HPP

#include "FilesystemError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ReadOnlyFilesystemError : public FilesystemError {

	  public:
		ReadOnlyFilesystemError(Filesystem::ErrorCode);
		ReadOnlyFilesystemError(const ReadOnlyFilesystemError&);

		REDSTRAIN_DECLARE_ERROR(ReadOnlyFilesystemError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_READONLYFILESYSTEMERROR_HPP */
