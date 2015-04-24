#ifndef REDSTRAIN_MOD_PLATFORM_FILETELLERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_FILETELLERROR_HPP

#include "FileError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API FileTellError : public FileError {

	  public:
		FileTellError(File::Direction, File::ErrorCode);
		FileTellError(const FileTellError&);

		REDSTRAIN_DECLARE_ERROR(FileTellError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILETELLERROR_HPP */
