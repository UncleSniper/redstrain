#ifndef REDSTRAIN_MOD_PLATFORM_FILESYSTEMERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILESYSTEMERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "Filesystem.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FilesystemError : public error::IOError {

	  protected:
		const Filesystem::ErrorCode code;

	  public:
		FilesystemError(Filesystem::ErrorCode);
		FilesystemError(const FilesystemError&);

		inline Filesystem::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(FilesystemError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILESYSTEMERROR_HPP */
