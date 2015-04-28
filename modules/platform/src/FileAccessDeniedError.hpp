#ifndef REDSTRAIN_MOD_PLATFORM_FILEACCESSDENIEDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILEACCESSDENIEDERROR_HPP

#include "FilesystemError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileAccessDeniedError : public FilesystemError {

	  public:
		FileAccessDeniedError(Filesystem::ErrorCode);
		FileAccessDeniedError(const FileAccessDeniedError&);

		REDSTRAIN_DECLARE_ERROR(FileAccessDeniedError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILEACCESSDENIEDERROR_HPP */
