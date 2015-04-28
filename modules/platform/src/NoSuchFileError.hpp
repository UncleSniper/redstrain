#ifndef REDSTRAIN_MOD_PLATFORM_NOSUCHFILEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_NOSUCHFILEERROR_HPP

#include "FilesystemError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NoSuchFileError : public FilesystemError {

	  public:
		NoSuchFileError(Filesystem::ErrorCode);
		NoSuchFileError(const NoSuchFileError&);

		REDSTRAIN_DECLARE_ERROR(NoSuchFileError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NOSUCHFILEERROR_HPP */
